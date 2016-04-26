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

class RimSummaryPlot;
class RicDropEnabledMainWindow;
class RifReaderEclipseSummary;

//==================================================================================================
///  
///  
//==================================================================================================
class RimSummaryPlotCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:
    RimSummaryPlotCollection();
    virtual ~RimSummaryPlotCollection();

    void showPlotWindow();
    void hidePlotWindow();

    RifReaderEclipseSummary* fileReader(const QString& eclipseCase);

    void createDockWindowsForAllPlots();
    void deletePlot(RimSummaryPlot* graphPlot);
    RimSummaryPlot* createAppendPlot(const QString& name);

    caf::PdmField<bool>                     showWindow;

    virtual caf::PdmFieldHandle*            objectToggleField() { return &showWindow; }
    virtual void                                        fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue);
    virtual void                                        initAfterRead();


private:
    caf::PdmChildArrayField<RimSummaryPlot*> m_graphPlots;

private:
    RifReaderEclipseSummary* createReader(const QString& eclipseCase);

    QDockWidget* dockWidgetFromPlot(RimSummaryPlot* graphPlot);
    void createDockWidget(RimSummaryPlot* graphPlot);
    void eraseDockWidget(RimSummaryPlot* graphPlot);

private:
    RicDropEnabledMainWindow* m_plotMainWindow;

    std::vector<QPointer<QDockWidget> > additionalProjectViews;

    // Map from path to case to summary file reader objects
    std::map<QString, RifReaderEclipseSummary*> m_summaryFileReaders;
};
