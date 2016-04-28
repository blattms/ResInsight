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

#include "RicAddSummaryCurveFeature.h"

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

CAF_CMD_SOURCE_INIT(RicAddSummaryCurveFeature, "RicAddSummaryCurveFeature");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RicAddSummaryCurveFeature::isCommandEnabled()
{
    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicAddSummaryCurveFeature::onActionTriggered(bool isChecked)
{
    caf::PdmObject* pdmObject = dynamic_cast<caf::PdmObject*>(caf::SelectionManager::instance()->selectedItem());
    if (pdmObject)
    {
        RimSummaryPlot* summaryPlot = NULL;
        pdmObject->firstAnchestorOrThisOfType(summaryPlot);
        if (summaryPlot)
        {
            summaryPlot->summaryCurves.push_back(new RimSummaryCurve);
            summaryPlot->updateConnectedEditors();
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicAddSummaryCurveFeature::setupActionLook(QAction* actionToSetup)
{
    //actionToSetup->setIcon(QIcon(":/CrossSection16x16.png"));
    actionToSetup->setText("Add (Summary Curve)");
}

