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

#include "RicCreateGraphPlotMainWindowFeature.h"

#include "RiaApplication.h"


#include "RicDropEnabledMainWindow.h"

#include "RifReaderEclipseSummary.h"

#include "RimEclipseResultCase.h"
#include "RimProject.h"
#include "RimSummaryCurve.h"
#include "RimSummaryPlot.h"
#include "RimSummaryPlotCollection.h"

#include "RiuResultQwtPlot.h"

#include "cafSelectionManager.h"

#include "cvfBase.h"
#include "cvfColor3.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QEvent>
#include <QMainWindow>

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
    RimProject* proj = RiaApplication::instance()->project();
    if (proj)
    {
        RimEclipseResultCase* eclipseResultCase = dynamic_cast<RimEclipseResultCase*>(caf::SelectionManager::instance()->selectedItem());
        
        RimSummaryPlotCollection* graphPlotCollection = proj->graphPlotCollection();
        graphPlotCollection->showPlotWindow();

        if (eclipseResultCase)
        {
            RifReaderEclipseSummary* eclipseSummaryReader = graphPlotCollection->getOrCreateSummaryFileReader(eclipseResultCase);
            if (eclipseSummaryReader)
            {
                std::vector<std::string> allVariableNames = eclipseSummaryReader->variableNames();

                RimSummaryPlot* graphPlot = NULL;

                for (size_t i = 0; i < allVariableNames.size(); i++)
                {
                    if (i % 10 == 0)
                    {
                        graphPlot = graphPlotCollection->createAppendPlot("My Plot");
                    }

                    RimSummaryCurve* summaryCurve = new RimSummaryCurve;
                    summaryCurve->m_eclipseCase = eclipseResultCase;
                    summaryCurve->m_variableName = QString::fromStdString(allVariableNames[i]);

                    graphPlot->summaryCurves.push_back(summaryCurve);
                }

                graphPlotCollection->updateConnectedEditors();
                graphPlotCollection->createDockWindowsForAllPlots();
                graphPlotCollection->redrawAllPlots();
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicCreateGraphPlotMainWindowFeature::setupActionLook(QAction* actionToSetup)
{
    //actionToSetup->setIcon(QIcon(":/CrossSection16x16.png"));
    actionToSetup->setText("Show Summary Plots");
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RiuResultQwtPlot* RicCreateGraphPlotMainWindowFeature::createPlotWidget(QWidget* parent)
{
    RiuResultQwtPlot* qwtPlot = new RiuResultQwtPlot(parent);

    std::vector<double> plotValues;
    plotValues.push_back(20);
    plotValues.push_back(30);
    plotValues.push_back(35);
    plotValues.push_back(25);
    plotValues.push_back(24);
    plotValues.push_back(22);
    plotValues.push_back(12);
    plotValues.push_back(30);
    plotValues.push_back(40);
    plotValues.push_back(250);

    std::vector<double> frameTimes;
    std::vector<double> values;

    for (size_t i = 0; i < 10; i++)
    {
        frameTimes.push_back(i);
        values.push_back(plotValues[i % plotValues.size()]);
    }

    qwtPlot->addCurve("MyCurve", cvf::Color3f::RED, frameTimes, values);

    return qwtPlot;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QDockWidget* RicCreateGraphPlotMainWindowFeature::createDockWidget(RicDropEnabledMainWindow* mainWindow, Qt::DockWidgetArea area, const QString& name)
{
    QDockWidget* dockWidget = new QDockWidget(name, mainWindow);
    dockWidget->setObjectName(name);

    RiuResultQwtPlot* qwtPlot = createPlotWidget(mainWindow);
    dockWidget->setWidget(qwtPlot);

    mainWindow->addDockWidget(area, dockWidget);

    connect(dockWidget, SIGNAL(topLevelChanged(bool)), mainWindow, SLOT(dragStarted(bool)));
    connect(dockWidget, SIGNAL(dockLocationChanged (Qt::DockWidgetArea)), mainWindow, SLOT(dragEnded()));

    return dockWidget;
}

