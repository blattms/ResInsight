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

#include "RimGraphPlotCollection.h"

#include "RimGraphPlot.h"
#include "GraphPlotCommands/RicDropEnabledMainWindow.h"


CAF_PDM_SOURCE_INIT(RimGraphPlotCollection, "RimGraphPlotCollection");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimGraphPlotCollection::RimGraphPlotCollection()
{
    CAF_PDM_InitObject("Graph Plots", "", "", "");

    CAF_PDM_InitFieldNoDefault(&graphPlots, "GraphPlots", "",  "", "", "");
    graphPlots.uiCapability()->setUiHidden(true);

    m_plotMainWindow = NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimGraphPlotCollection::~RimGraphPlotCollection()
{
    graphPlots.deleteAllChildObjects();

    m_plotMainWindow->close();
    m_plotMainWindow->deleteLater();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimGraphPlotCollection::showPlotWindow()
{
    if (!m_plotMainWindow)
    {
        m_plotMainWindow = new RicDropEnabledMainWindow;
        m_plotMainWindow->setDockNestingEnabled(true);
    }

    m_plotMainWindow->showNormal();
    m_plotMainWindow->raise();
}
