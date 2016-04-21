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

#include "RimGraphPlot.h"
#include "RimSummaryCurve.h"
#include "RiuResultQwtPlot.h"


CAF_PDM_SOURCE_INIT(RimGraphPlot, "GraphPlot");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimGraphPlot::RimGraphPlot()
{
    CAF_PDM_InitObject("Graph Plot", "", "", "");

    CAF_PDM_InitFieldNoDefault(&summaryCurves, "SummaryCurves", "",  "", "", "");
    summaryCurves.uiCapability()->setUiHidden(true);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimGraphPlot::~RimGraphPlot()
{
    deletePlotWidget();

    summaryCurves.deleteAllChildObjects();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RimGraphPlot::widget()
{
    return m_qwtPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RimGraphPlot::createPlotWidget(QWidget* parent)
{
    assert(m_qwtPlot.isNull());

    m_qwtPlot = new RiuResultQwtPlot(parent);

    return m_qwtPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimGraphPlot::deletePlotWidget()
{
    if (m_qwtPlot)
    {
        m_qwtPlot->deleteLater();
        m_qwtPlot = NULL;
    }
}
