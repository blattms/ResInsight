/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016 Statoil ASA
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

#include "RimSummaryPlot.h"

#include "RimSummaryCurve.h"
#include "RimSummaryPlotCollection.h"

#include "RiuResultQwtPlot.h"
#include "RiuSelectionColors.h"

#include "cvfBase.h"
#include "cvfColor3.h"

#include <QDateTime>


CAF_PDM_SOURCE_INIT(RimSummaryPlot, "GraphPlot");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryPlot::RimSummaryPlot()
{
    CAF_PDM_InitObject("Graph Plot", "", "", "");

    CAF_PDM_InitFieldNoDefault(&summaryCurves, "SummaryCurves", "",  "", "", "");
    summaryCurves.uiCapability()->setUiHidden(true);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryPlot::~RimSummaryPlot()
{
    deletePlotWidget();

    summaryCurves.deleteAllChildObjects();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RimSummaryPlot::widget()
{
    return m_qwtPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RimSummaryPlot::createPlotWidget(QWidget* parent)
{
    assert(m_qwtPlot.isNull());

    m_qwtPlot = new RiuResultQwtPlot(parent);

    return m_qwtPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlot::deletePlotWidget()
{
    if (m_qwtPlot)
    {
        m_qwtPlot->deleteLater();
        m_qwtPlot = NULL;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RiuResultQwtPlot* RimSummaryPlot::qwtPlot()
{
    return m_qwtPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryPlot::redrawAllCurves()
{
    m_qwtPlot->deleteAllCurves();

    for (size_t i = 0; i < summaryCurves.size(); i++)
    {
        RimSummaryCurve* curve = summaryCurves[i];

        std::vector<QDateTime> dateTimes;
        std::vector<double> values;

        curve->curveData(&dateTimes, &values);

        cvf::Color3f curveColor = RiuSelectionColors::curveColorFromTable();

        m_qwtPlot->addCurve(curve->m_variableName(), curveColor, dateTimes, values);
    }
}
