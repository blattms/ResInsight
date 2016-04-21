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

#pragma once

#include "cafCmdFeature.h"
#include "cafCmdExecuteCommand.h"
#include "cafPdmPointer.h"


class RiuResultQwtPlot;
class QDockWidget;
class RicDropEnabledMainWindow;

//==================================================================================================
/// 
//==================================================================================================
class RicCreateGraphPlotMainWindowFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    // Overrides
    virtual bool isCommandEnabled();
    virtual void onActionTriggered( bool isChecked );
    virtual void setupActionLook( QAction* actionToSetup );

private:
    // TODO: Remove these when prototyping stage is completed
    RiuResultQwtPlot*   createPlotWidget(QWidget* parent);
    QDockWidget*        createDockWidget(RicDropEnabledMainWindow* mainWindow, Qt::DockWidgetArea area, const QString& name);
};


