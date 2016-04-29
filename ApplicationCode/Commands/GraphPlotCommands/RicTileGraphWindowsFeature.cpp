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

#include "RicTileGraphWindowsFeature.h"

#include "RiaApplication.h"
#include "RimProject.h"
#include "RimSummaryPlotCollection.h"

#include <QAction>
#include <QMainWindow>

CAF_CMD_SOURCE_INIT(RicTileGraphWindowsFeature, "RicTileGraphWindowsFeature");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RicTileGraphWindowsFeature::isCommandEnabled()
{
    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicTileGraphWindowsFeature::onActionTriggered(bool isChecked)
{
    RimProject* proj = RiaApplication::instance()->project();
    if (proj)
    {
        RimSummaryPlotCollection* plotCollection = proj->graphPlotCollection();

        QMainWindow* graphMainWindow = plotCollection->windowWithGraphPlots();
        if (graphMainWindow)
        {
            QList<QDockWidget*> dockWidgets = graphMainWindow->findChildren<QDockWidget*>();

            for (int i = 0; i < dockWidgets.size(); i++)
            {
                graphMainWindow->removeDockWidget(dockWidgets[i]);
            }

            QDockWidget* previousWidget = NULL;


            for (int i = 0; i < dockWidgets.size(); i++)
            {
                graphMainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidgets.at(i));
                dockWidgets.at(i)->show();

                if (i % 2 != 0 && previousWidget)
                {
                    graphMainWindow->splitDockWidget(previousWidget, dockWidgets.at(i), Qt::Horizontal);
                }

                previousWidget = dockWidgets.at(i);
            }


/*
Prototype layout 


            int horizontalCount = 4;

            for (int i = 0; i < dockWidgets.size(); i++)
            {
                graphMainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidgets.at(i));
                dockWidgets.at(i)->show();

                if (i < horizontalCount)
                {
                    if (i == 1)
                    {
                        graphMainWindow->splitDockWidget(dockWidgets.at(0), dockWidgets.at(i), Qt::Horizontal);
                    }
                    else if (i == 2)
                    {
                        graphMainWindow->splitDockWidget(dockWidgets.at(0), dockWidgets.at(i), Qt::Horizontal);
                    }
                    else if (i == 3)
                    {
                        graphMainWindow->splitDockWidget(dockWidgets.at(2), dockWidgets.at(i), Qt::Horizontal);
                    }
                }
                else
                {
                    graphMainWindow->splitDockWidget(previousWidget, dockWidgets.at(i), Qt::Horizontal);

                }
*/


/*
            for (int i = 0; i < dockWidgets.size(); i++)
            {
                dockWidgets.at(i)->widget()->resize(100, 50);
            }
*/
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicTileGraphWindowsFeature::setupActionLook(QAction* actionToSetup)
{
    //actionToSetup->setIcon(QIcon(":/CrossSection16x16.png"));
    actionToSetup->setText("Tile Graph Windows");
}

