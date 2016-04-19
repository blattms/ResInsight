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


#include "RicDropEnabledMainWindow.h"

#include <QDockWidget>
#include <QEvent>
#include <QDebug>
#include <QApplication>

#include <iostream>

QDockWidget* RicDropEnabledMainWindow::dw = NULL; 




//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RicDropEnabledMainWindow::RicDropEnabledMainWindow(QWidget *parent /*= 0*/, Qt::WindowFlags flags /*= 0*/)
    : QMainWindow(parent, flags)
{

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RicDropEnabledMainWindow::~RicDropEnabledMainWindow()
{

}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicDropEnabledMainWindow::handleDockWidgetDrop()
{
    if (dw)
    {
        QRect globalDragGeo;
        {
            QRect dragWindowGeo = dw->geometry();
            globalDragGeo.setTopLeft(dragWindowGeo.topLeft());
            globalDragGeo.setBottomRight(dragWindowGeo.bottomRight());
        }

        QWidgetList wList = QApplication::allWidgets();
        foreach(QWidget* w, wList)
        {
            RicDropEnabledMainWindow* mainWindow = dynamic_cast<RicDropEnabledMainWindow*>(w);
            if (mainWindow && dw->parent() != mainWindow)
            {
                QRect globalMainWindowGeo;
                {
                    QRect mainWindowGeo = mainWindow->geometry();
                    globalMainWindowGeo.setTopLeft(mainWindowGeo.topLeft());
                    globalMainWindowGeo.setBottomRight(mainWindowGeo.bottomRight());
                }

                if (globalMainWindowGeo.contains(globalDragGeo))
                {
                    //dw->setParent(mainWindow);

                    QMainWindow* parentMain = dynamic_cast<QMainWindow*>(dw->parent());
                    if (parentMain)
                    {
                        parentMain->update();
                    }

                    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dw);
                    dw = NULL;

                    return;
                }
            }
        }
    }
}

void RicDropEnabledMainWindow::dragStarted(bool started)
{
    if(started)
    {
        if(QDockWidget* dock = qobject_cast<QDockWidget*>(sender()))
            dw = dock;
        else
            dw = NULL;
    }
}

void RicDropEnabledMainWindow::enterEvent(QEvent* event)
{
    handleDockWidgetDrop();
}

void RicDropEnabledMainWindow::dragEnded()
{
    handleDockWidgetDrop();

    dw = NULL;
}
