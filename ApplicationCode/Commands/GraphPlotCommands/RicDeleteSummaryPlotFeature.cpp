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

#include "RicDeleteSummaryPlotFeature.h"

#include "RiaApplication.h"

#include "RicDropEnabledMainWindow.h"

#include "RimEclipseCase.h"
#include "RimSummaryPlot.h"
#include "RimSummaryPlotCollection.h"
#include "RimProject.h"
#include "RimSummaryCurve.h"
#include "RiuResultQwtPlot.h"

#include "cafSelectionManager.h"

#include "cvfBase.h"
#include "cvfColor3.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QEvent>
#include <QMainWindow>

CAF_CMD_SOURCE_INIT(RicDeleteSummaryPlotFeature, "RicDeleteSummaryPlotFeature");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RicDeleteSummaryPlotFeature::isCommandEnabled()
{
    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicDeleteSummaryPlotFeature::onActionTriggered(bool isChecked)
{
    RimSummaryPlot* summaryPlot = dynamic_cast<RimSummaryPlot*>(caf::SelectionManager::instance()->selectedItem());
    if (summaryPlot)
    {
        RimSummaryPlotCollection* graphPlotCollection = NULL;
        summaryPlot->firstAnchestorOrThisOfType(graphPlotCollection);
        if (graphPlotCollection)
        {
            graphPlotCollection->deletePlot(summaryPlot);
            graphPlotCollection->updateConnectedEditors();
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicDeleteSummaryPlotFeature::setupActionLook(QAction* actionToSetup)
{
    //actionToSetup->setIcon(QIcon(":/CrossSection16x16.png"));
    actionToSetup->setText("Delete (Summary Plot)");
}

