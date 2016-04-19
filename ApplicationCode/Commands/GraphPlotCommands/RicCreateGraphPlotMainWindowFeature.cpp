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
#include <QMainWindow>
#include <QDockWidget>
#include <QEvent>

#include "RiuResultQwtPlot.h"

#include "cvfBase.h"
#include "cvfColor3.h"
#include "RicDropEnabledMainWindow.h"
#include "QApplication"

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
    RicDropEnabledMainWindow* mainWindow = new RicDropEnabledMainWindow;

    mainWindow->setDockNestingEnabled(true);
    mainWindow->showNormal();

    int plotId = 0;

    for (size_t i = 0; i < 5; i++)
    {
        createDockWidget(mainWindow, Qt::LeftDockWidgetArea, QString("Plot %1").arg(plotId++));
    }

    for (size_t i = 0; i < 5; i++)
    {
        createDockWidget(mainWindow, Qt::RightDockWidgetArea, QString("Plot %1").arg(plotId++));
    }

/*
    {
        QDockWidget* dockWidget = new QDockWidget("Plots", mainWindow);
        dockWidget->setObjectName("dockWidget");

        mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    }

    {
        QDockWidget* dockWidget = new QDockWidget("Plots Test", mainWindow);
        dockWidget->setObjectName("dockWidget");

        mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    }

    {
        QDockWidget* dockWidget = new QDockWidget("Plots Test 1", mainWindow);
        dockWidget->setObjectName("dockWidget");

        mainWindow->addDockWidget(Qt::TopDockWidgetArea, dockWidget);
    }

    {
        QDockWidget* dockWidget = new QDockWidget("Plots Test 2", mainWindow);
        dockWidget->setObjectName("dockWidget");

        mainWindow->addDockWidget(Qt::AllDockWidgetAreas, dockWidget);
    }

    {
        QDockWidget* dockWidget = new QDockWidget("Plots Test 3", mainWindow);
        dockWidget->setObjectName("dockWidget");

        mainWindow->addDockWidget(Qt::AllDockWidgetAreas, dockWidget);
    }
*/


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


class EventDebugger : public QObject
 {
 public:
     EventDebugger() {} ;

 protected:
     bool eventFilter(QObject *obj, QEvent *event);
 };

 bool EventDebugger::eventFilter(QObject *obj, QEvent *event)
 {
    //qDebug() << event->type();

    if (event->type() == 71)
    {
        RicDropEnabledMainWindow::handleDockWidgetDrop();
    }

    return QObject::eventFilter(obj, event);

/*
     if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
         return true;
     } else {
         // standard event processing
     }
*/
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

    connect(dockWidget, SIGNAL(topLevelChanged(bool)) , mainWindow, SLOT(dragStarted(bool)));
    connect(dockWidget, SIGNAL(dockLocationChanged (Qt::DockWidgetArea)) , mainWindow, SLOT(dragEnded()));

/*
    EventDebugger* evDebug = new EventDebugger;
    dockWidget->installEventFilter(evDebug);
*/

    return dockWidget;
}

