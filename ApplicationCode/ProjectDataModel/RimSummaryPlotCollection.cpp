/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016  Statoil ASA
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

#include "RimSummaryPlotCollection.h"

#include "GraphPlotCommands/RicDropEnabledMainWindow.h"

#include "RifEclipseSummaryTools.h"
#include "RifReaderEclipseSummary.h"

#include "RimEclipseResultCase.h"
#include "RimSummaryPlot.h"

#include <QDockWidget>


CAF_PDM_SOURCE_INIT(RimSummaryPlotCollection, "RimGraphPlotCollection");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryPlotCollection::RimSummaryPlotCollection()
{
    CAF_PDM_InitObject("Graph Plots", "", "", "");

    CAF_PDM_InitField(&showWindow, "ShowWindow", true, "Show 2D Plot windows", "", "", "");
    showWindow.uiCapability()->setUiHidden(true);

    CAF_PDM_InitFieldNoDefault(&m_graphPlots, "GraphPlots", "",  "", "", "");
    m_graphPlots.uiCapability()->setUiHidden(true);

    m_plotMainWindow = NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryPlotCollection::~RimSummaryPlotCollection()
{
    m_graphPlots.deleteAllChildObjects();

    m_plotMainWindow->close();
    m_plotMainWindow->deleteLater();

    for (auto it = m_summaryFileReaders.begin(); it != m_summaryFileReaders.end(); it++)
    {
        delete it->second;
    }
    m_summaryFileReaders.clear();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::showPlotWindow()
{
    if (!m_plotMainWindow)
    {
        m_plotMainWindow = new RicDropEnabledMainWindow;
        m_plotMainWindow->setDockNestingEnabled(true);
    }

    m_plotMainWindow->showNormal();
    m_plotMainWindow->raise();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RifReaderEclipseSummary* RimSummaryPlotCollection::getOrCreateSummaryFileReader(const QString& eclipseCase)
{
    auto it = m_summaryFileReaders.find(eclipseCase);
    if (it != m_summaryFileReaders.end())
    {
        return it->second;
    }
    else
    {
        return createSummaryFileReader(eclipseCase);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RifReaderEclipseSummary* RimSummaryPlotCollection::getOrCreateSummaryFileReader(const RimEclipseResultCase* eclipseCase)
{
    if (!eclipseCase) return NULL;

    QString caseName = eclipseCase->gridFileName();
    QString caseNameWithNoExtension = caseName.remove(".egrid", Qt::CaseInsensitive);

    QString caseNameAbsPath = caseNameWithNoExtension.replace("/", "\\");

    return this->getOrCreateSummaryFileReader(caseNameAbsPath);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RifReaderEclipseSummary* RimSummaryPlotCollection::createSummaryFileReader(const QString& eclipseCase)
{
    std::string headerFile;
    bool isFormatted = false;
    RifEclipseSummaryTools::findSummaryHeaderFile(eclipseCase.toStdString(), &headerFile, &isFormatted);

    std::vector<std::string> dataFiles = RifEclipseSummaryTools::findSummaryDataFiles(eclipseCase.toStdString());

    RifReaderEclipseSummary* reader = new RifReaderEclipseSummary;
    if (!reader->open(headerFile, dataFiles))
    {
        delete reader;

        return NULL;
    }
    else
    {
        m_summaryFileReaders.insert(std::make_pair(eclipseCase, reader));
        return reader;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::createDockWindowsForAllPlots()
{
    for (size_t i = 0; i < m_graphPlots.size(); i++)
    {
        if (!dockWidgetFromPlot(m_graphPlots[i]))
        {
            createDockWidget(m_graphPlots[i]);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QDockWidget* RimSummaryPlotCollection::dockWidgetFromPlot(RimSummaryPlot* graphPlot)
{
    foreach(QDockWidget* dockW, additionalProjectViews)
    {
        if (dockW && dockW->widget() == graphPlot->widget())
        {
            return dockW;
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::createDockWidget(RimSummaryPlot* graphPlot)
{
    assert(m_plotMainWindow != NULL);

    QDockWidget* dockWidget = new QDockWidget(QString("Plot Widget Tree (%1)").arg(additionalProjectViews.size() + 1), m_plotMainWindow);
    dockWidget->setObjectName("dockWidget");
    // dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = graphPlot->createPlotWidget(m_plotMainWindow);

    dockWidget->setWidget(widget);

    m_plotMainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    additionalProjectViews.push_back(dockWidget);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::eraseDockWidget(RimSummaryPlot* graphPlot)
{
    QDockWidget* dockW = dockWidgetFromPlot(graphPlot);
    if (dockW)
    {
        m_plotMainWindow->removeDockWidget(dockW);
        dockW->setWidget(NULL);
        dockW->deleteLater();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::deletePlot(RimSummaryPlot* graphPlot)
{
    m_graphPlots.removeChildObject(graphPlot);
    eraseDockWidget(graphPlot);

    graphPlot->deletePlotWidget();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryPlot* RimSummaryPlotCollection::createAppendPlot(const QString& name)
{
    RimSummaryPlot* graphPlot = new RimSummaryPlot;

    m_graphPlots.push_back(graphPlot);

    return graphPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::hidePlotWindow()
{
    if (m_plotMainWindow)
    {
        m_plotMainWindow->hide();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue)
{
    if (changedField == &showWindow)
    {
        if (showWindow)
        {
            showPlotWindow();
        }
        else
        {
            hidePlotWindow();
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlotCollection::initAfterRead()
{
    if (showWindow)
    {
        showPlotWindow();
    }
    else
    {
        hidePlotWindow();
    }
}
