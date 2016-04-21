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

#include "cafPdmObject.h"
#include "cafPdmField.h"
#include "cafPdmChildArrayField.h"

#include <QPointer>
#include <QDockWidget>

class RimGraphPlot;
class RicDropEnabledMainWindow;
class RifReaderEclipseSummary;

//==================================================================================================
///  
///  
//==================================================================================================
class RimGraphPlotCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:
    RimGraphPlotCollection();
    virtual ~RimGraphPlotCollection();

    void showPlotWindow();

    RifReaderEclipseSummary* fileReader(const QString& eclipseCase);

    void createDockWindowsForAllPlots();
    void deletePlot(RimGraphPlot* graphPlot);
    RimGraphPlot* createAppendPlot(const QString& name);

private:
    caf::PdmChildArrayField<RimGraphPlot*> m_graphPlots;

private:
    RifReaderEclipseSummary* createReader(const QString& eclipseCase);

    QDockWidget* dockWidgetFromPlot(RimGraphPlot* graphPlot);
    void createDockWidget(RimGraphPlot* graphPlot);
    void eraseDockWidget(RimGraphPlot* graphPlot);

private:
    RicDropEnabledMainWindow* m_plotMainWindow;

    std::vector<QPointer<QDockWidget> > additionalProjectViews;

    // Map from path to case to summary file reader objects
    std::map<QString, RifReaderEclipseSummary*> m_summaryFileReaders;
};
