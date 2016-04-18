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


CAF_PDM_SOURCE_INIT(RimGraphPlotCollection, "RimGraphPlotCollection");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimGraphPlotCollection::RimGraphPlotCollection()
{
    CAF_PDM_InitObject("Graph Plots", "", "", "");

    CAF_PDM_InitFieldNoDefault(&graphPlots, "GraphPlots", "",  "", "", "");
    graphPlots.uiCapability()->setUiHidden(true);

    graphPlots.push_back(new RimGraphPlot);
    graphPlots.push_back(new RimGraphPlot);
    graphPlots.push_back(new RimGraphPlot);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimGraphPlotCollection::~RimGraphPlotCollection()
{
    graphPlots.deleteAllChildObjects();
}
