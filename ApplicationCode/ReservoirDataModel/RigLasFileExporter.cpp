/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RigLasFileExporter.h"

#include "RigWellLogCurveData.h"
#include "RimCase.h"
#include "RimDefines.h"
#include "RimWellLogCurve.h"
#include "RimWellLogExtractionCurve.h"

#include "cvfAssert.h"

#include "laswell.hpp"

#include <QDir>
#include <QMessageBox>

#include <cmath> // Needed for HUGE_VAL on Linux


namespace NRLib
{
    class LasWell;
};


class SingleChannelData
{
public:
    SingleChannelData(const std::string& channelName, const std::string& unit, const std::string& comment, const RigWellLogCurveData* curveData)
        : m_channelName(channelName),
        m_unit(unit),
        m_comment(comment),
        m_curveData(curveData)
    {
    }

    void appendDataToLasFile(NRLib::LasWell* lasFile, double absentValue) const
    {
        CVF_ASSERT(lasFile);

        std::vector<double> wellLogValues = m_curveData->xValues();
        for (size_t vIdx = 0; vIdx < wellLogValues.size(); vIdx++)
        {
            double value = wellLogValues[vIdx];
            if (value == HUGE_VAL || value == -HUGE_VAL || value != value)
            {
                wellLogValues[vIdx] = absentValue;
            }
        }

        lasFile->AddLog(m_channelName, m_unit, m_comment, wellLogValues);
    }

    std::string channelName() const
    {
        return m_channelName;
    }

    const RigWellLogCurveData* curveData() const
    {
        return m_curveData;
    }

private:
    std::string m_channelName;
    std::string m_unit;
    std::string m_comment;

    const RigWellLogCurveData* m_curveData;
};

class SingleLasFileMetaData
{
public:
    SingleLasFileMetaData()
        : m_minimumCurveValue(HUGE_VAL),
        m_rkbDiff(HUGE_VAL)
    {
    }

    void setWellName(const QString& wellName)
    {
        m_wellName = wellName;
    }

    void setCaseName(const QString& caseName)
    {
        m_caseName = caseName;
    }

    void setDate(const QString& date)
    {
        m_date = date;
    }

    void setRkbDiff(double rkbDiff)
    {
        m_rkbDiff = rkbDiff;
    }

    void addLogData(const std::string& channelName, const std::string& unit, const std::string& comment, const RigWellLogCurveData* curveData)
    {
        m_logCurveData.push_back(SingleChannelData(channelName, unit, comment, curveData));

        for (double xValue : curveData->xValues())
        {
            if (xValue < m_minimumCurveValue)
            {
                m_minimumCurveValue = xValue;
            }
        }
    }

    std::string generateFilename() const
    {
        QString f;
        QString separator("-");

        if (!m_wellName.isEmpty())
        {
            f += m_wellName;
        }

        if (!m_caseName.isEmpty())
        {
            if (!f.isEmpty()) f += separator;
            f += m_caseName;
        }

        // Add property name if only one curve is exported
        if (m_logCurveData.size() == 1)
        {
            if (!f.isEmpty()) f += separator;
            f += QString::fromStdString(m_logCurveData[0].channelName());
        }

        if (!m_date.isEmpty())
        {
            if (!f.isEmpty()) f += separator;
            f += m_date;
        }

        QString cleanFileName = f.trimmed();
        cleanFileName.replace(".", "_");
        cleanFileName.replace(",", "_");
        cleanFileName.replace(":", "_");
        cleanFileName.replace(";", "_");
        cleanFileName.replace(" ", "_");
        cleanFileName.replace("/", "_");
        cleanFileName.replace(QRegExp("_+"), "_");

        cleanFileName += ".las";

        return cleanFileName.toStdString();
    }

    void appendDataToLasFile(NRLib::LasWell* lasFile) const
    {
        if (m_logCurveData.size() == 0) return;

        lasFile->addWellInfo("WELL", m_wellName.toStdString());

        QString wellLogDate = m_date;
        wellLogDate.replace(".", "_");
        wellLogDate.replace(" ", "_");

        lasFile->addWellInfo("DATE", wellLogDate.toStdString());

        const RigWellLogCurveData* firstCurveData = curveDataForFirstCurve();

        if (firstCurveData->depthUnit() == RimDefines::UNIT_METER)
        {
            lasFile->AddLog("DEPTH", "M", "Depth in meters", firstCurveData->measuredDepths());
        }
        else if (firstCurveData->depthUnit() == RimDefines::UNIT_FEET)
        {
            lasFile->AddLog("DEPTH", "FT", "Depth in feet", firstCurveData->measuredDepths());
        }

        if (firstCurveData->tvDepths().size())
        {
            lasFile->AddLog("TVDMSL", "M", "True vertical depth in meters", firstCurveData->tvDepths());

            if (m_rkbDiff != HUGE_VAL)
            {
                // Export True Vertical Depth Rotary Kelly Bushing - TVDRKB
                std::vector<double> tvdrkbValues = firstCurveData->tvDepths();
                for (auto& value : tvdrkbValues)
                {
                    value += m_rkbDiff;
                }

                if (firstCurveData->depthUnit() == RimDefines::UNIT_METER)
                {
                    lasFile->AddLog("TVDRKB", "M", "True vertical depth (Rotary Kelly Bushing)", tvdrkbValues);
                }
                else if (firstCurveData->depthUnit() == RimDefines::UNIT_FEET)
                {
                    lasFile->AddLog("TVDRKB", "FT", "True vertical depth (Rotary Kelly Bushing)", tvdrkbValues);
                }
            }
        }

        double minDepth = 0.0;
        double maxDepth = 0.0;
        firstCurveData->calculateMDRange(&minDepth, &maxDepth);

        lasFile->setStartDepth(minDepth);
        lasFile->setStopDepth(maxDepth);

        if (firstCurveData->depthUnit() == RimDefines::UNIT_METER)
        {
            lasFile->setDepthUnit("M");
        }
        else if (firstCurveData->depthUnit() == RimDefines::UNIT_FEET)
        {
            lasFile->setDepthUnit("FT");
        }

        double absentValue = SingleLasFileMetaData::createAbsentValue(m_minimumCurveValue);
        lasFile->SetMissing(absentValue);

        for (auto curveData : m_logCurveData)
        {
            curveData.appendDataToLasFile(lasFile, absentValue);
        }
    }


private:
    const RigWellLogCurveData* curveDataForFirstCurve() const
    {
        CVF_ASSERT(m_logCurveData.size() > 0);

        return m_logCurveData[0].curveData();
    }

    static double createAbsentValue(double lowestDataValue)
    {
        double absentValue = -999.0;

        while (absentValue > lowestDataValue)
        {
            absentValue *= 10;
            absentValue -= 9;
        }

        return absentValue - 0.25;
    }

private:
    QString m_wellName;
    QString m_caseName;
    QString m_date;
    double m_rkbDiff;

    RimDefines::DepthUnitType m_depthUnit;
    std::vector<double> m_depthValues;

    std::vector<SingleChannelData> m_logCurveData;
    double m_minimumCurveValue;
};

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RigLasFileExporter::RigLasFileExporter(const std::vector<RimWellLogCurve*>& curves)
    : m_curves(curves),
    m_isResampleActive(false),
    m_resamplingInterval(1.0)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RigLasFileExporter::setResamplingInterval(double interval)
{
    m_isResampleActive = true;
    m_resamplingInterval = interval;
    m_resampledCurveDatas.clear();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RigLasFileExporter::writeToFolder(const QString& exportFolder)
{
    std::vector<SingleLasFileMetaData> lasFileDescriptions = createLasFileDescriptions(m_curves);

    for (auto lasFileDescr : lasFileDescriptions)
    {
        NRLib::LasWell lasFile;
        lasFile.setVersionInfo("2.0");

        lasFileDescr.appendDataToLasFile(&lasFile);
        if (m_isResampleActive)
        {
            lasFile.setDepthStep(m_resamplingInterval);
        }

        QDir dir(exportFolder);
        QString fileName = dir.absoluteFilePath(QString::fromStdString(lasFileDescr.generateFilename()));
        if (QFile::exists(fileName))
        {
            QString txt = QString("File %1 exists.\n\nDo you want to overwrite the file?").arg(fileName);
            int ret = QMessageBox::question(NULL, "LAS File Export",
                        txt,
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);

            if (ret != QMessageBox::Yes) continue;
        }

        std::vector<std::string> commentHeader;
        lasFile.WriteToFile(fileName.toStdString(), commentHeader);
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::vector<SingleLasFileMetaData> RigLasFileExporter::createLasFileDescriptions(const std::vector<RimWellLogCurve*>& curves)
{
    std::vector<RimWellLogCurve*> eclipseCurves;
    std::vector<RimWellLogCurve*> geoMechCurves;
    std::vector<RimWellLogCurve*> externalLasCurves;

    for (auto curve : curves)
    {
        RimWellLogExtractionCurve* extractionCurve = dynamic_cast<RimWellLogExtractionCurve*>(curve);
        if (extractionCurve)
        {
            if (extractionCurve->isEclipseCurve())
            {
                eclipseCurves.push_back(extractionCurve);
            }
            else
            {
                geoMechCurves.push_back(extractionCurve);
            }
        }
        else
        {
            externalLasCurves.push_back(curve);
        }
    }

    // External LAS files

    std::vector<SingleLasFileMetaData> lasFileDescriptions;

    appendLasFileDescriptions(externalLasCurves, &lasFileDescriptions);
    appendLasFileDescriptions(eclipseCurves, &lasFileDescriptions);
    appendLasFileDescriptions(geoMechCurves, &lasFileDescriptions);

    return lasFileDescriptions;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RigLasFileExporter::appendLasFileDescriptions(const std::vector<RimWellLogCurve*>& curves, std::vector<SingleLasFileMetaData>* lasFileDescriptions)
{
    CVF_ASSERT(lasFileDescriptions);

    struct CurveCollectionDefinition
    {
        CurveCollectionDefinition(const QString& wellName, const QString& caseName, const QString& date)
            : m_wellName(wellName),
            m_caseName(caseName),
            m_date(date)
        {}

        bool isEqual(RimWellLogCurve* curve, const QString& caseName)
        {
            if (m_wellName == curve->wellName() &&
                m_caseName == caseName &&
                m_date == curve->wellDate())
            {
                return true;
            }

            return false;
        }

        QString m_wellName;
        QString m_caseName;
        QString m_date;
    };

    std::vector<CurveCollectionDefinition> curveDefinitions;
    for (auto curve : curves)
    {
        QString caseName = caseNameFromCurve(curve);

        bool found = false;
        for (auto curveDef : curveDefinitions)
        {
            if (curveDef.isEqual(curve, caseName))
            {
                found = true;
            }
        }

        if (!found)
        {
            CurveCollectionDefinition curveDefCandidate(curve->wellName(), caseName, curve->wellDate());
            curveDefinitions.push_back(curveDefCandidate);
        }
    }

    for (auto curveDef : curveDefinitions)
    {
        SingleLasFileMetaData singleLasFileMeta;
        singleLasFileMeta.setWellName(curveDef.m_wellName);
        singleLasFileMeta.setCaseName(curveDef.m_caseName);
        singleLasFileMeta.setDate(curveDef.m_date);

        for (auto curve : curves)
        {

            if (curveDef.isEqual(curve, caseNameFromCurve(curve)))
            {
                singleLasFileMeta.setRkbDiff(rkbDiff(curve));

                const RigWellLogCurveData* curveData = nullptr;
                if (m_isResampleActive)
                {
                    cvf::ref<RigWellLogCurveData> resampledData = curve->curveData()->calculateResampledCurveData(m_resamplingInterval);
                    m_resampledCurveDatas.push_back(resampledData.p());

                    curveData = resampledData.p();
                }
                else
                {
                    curveData = curve->curveData();
                }
                singleLasFileMeta.addLogData(curve->wellLogChannelName().toStdString(), "NO_UNIT", "", curveData);
            }
        }

        lasFileDescriptions->push_back(singleLasFileMeta);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString RigLasFileExporter::caseNameFromCurve(RimWellLogCurve* curve)
{
    QString caseName;

    RimWellLogExtractionCurve* extractionCurve = dynamic_cast<RimWellLogExtractionCurve*>(curve);
    if (extractionCurve)
    {
        caseName = extractionCurve->caseName();
    }
    else
    {
        caseName = "unnamed";
    }

    return caseName;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
double RigLasFileExporter::rkbDiff(RimWellLogCurve* curve)
{
    RimWellLogExtractionCurve* extractionCurve = dynamic_cast<RimWellLogExtractionCurve*>(curve);
    if (extractionCurve)
    {
        return extractionCurve->rkbDiff();
    }

    return HUGE_VAL;
}

