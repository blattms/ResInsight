/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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

#include "RicCreateGraphPlotMainWindowFeature.h"


#include <QAction>

CAF_CMD_SOURCE_INIT(RicCreateGraphPlotMainWindowFeature, "RicCreateGraphPlotMainWindowFeature");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RicCreateGraphPlotMainWindowFeature::isCommandEnabled()
{
    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicCreateGraphPlotMainWindowFeature::onActionTriggered(bool isChecked)
{
/*
    std::vector<caf::PdmObjectHandle*> collection;
    caf::SelectionManager::instance()->objectsByType(&collection);
    CVF_ASSERT(collection.size() == 1);

    RimCrossSectionCollection* crossSectionCollection = NULL;
    collection[0]->firstAnchestorOrThisOfType(crossSectionCollection);

    CVF_ASSERT(crossSectionCollection);

    RicAppendCrossSectionFeatureCmd* cmd = new RicAppendCrossSectionFeatureCmd(crossSectionCollection);
    caf::CmdExecCommandManager::instance()->processExecuteCommand(cmd);
*/
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicCreateGraphPlotMainWindowFeature::setupActionLook(QAction* actionToSetup)
{
    //actionToSetup->setIcon(QIcon(":/CrossSection16x16.png"));
    actionToSetup->setText("New Graph Mainwindow");
}

