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


#pragma once

#include "cafPdmObject.h"
#include "cafPdmField.h"
#include "cafPdmChildArrayField.h"
#include "cafAppEnum.h"
#include "cafPdmChildField.h"

#include "RimDefines.h"
#include "RimViewWindow.h"

#include <QPointer>

#include <memory>

class RiuSummaryQwtPlot;
class RimSummaryCurve;
class RimSummaryCurveFilter;
class RimSummaryYAxisProperties;
class RimSummaryTimeAxisProperties;
class PdmUiTreeOrdering;

class QwtPlotCurve;

//==================================================================================================
///  
///  
//==================================================================================================
class RimSummaryPlot : public RimViewWindow
{
    CAF_PDM_HEADER_INIT;

public:
    RimSummaryPlot();
    virtual ~RimSummaryPlot();

    void                                            setDescription(const QString& description);
    void                                            addCurve(RimSummaryCurve* curve);
    void                                            addCurveFilter(RimSummaryCurveFilter* curveFilter);

    RimSummaryCurve*                                findRimCurveFromQwtCurve(const QwtPlotCurve* curve) const;
    
    void                                            loadDataAndUpdate();

    void                                            handleViewerDeletion();
    void                                            updateCaseNameHasChanged();

    QWidget*                                        viewer();

    void                                            setZoomWindow(const QRectF& zoomWindow);
    virtual void                                    zoomAll() override;
    void                                            updateZoomInQwt();
    void                                            disableAutoZoom();

    void                                            updateAxes();
    bool                                            isLogarithmicScaleEnabled(RimDefines::PlotAxis plotAxis) const;

protected:
    // Overridden PDM methods
    virtual caf::PdmFieldHandle*                    objectToggleField()    { return &m_showWindow; }
    virtual caf::PdmFieldHandle*                    userDescriptionField() { return &m_userName; }
    virtual void                                    fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue) override;
    virtual void                                    setupBeforeSave() override;
    virtual void                                    defineUiTreeOrdering(caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "") override;

    virtual QImage                                  snapshotWindowContent() override;

private:
    void                                            updateViewerWidget();
    void                                            updateViewerWidgetWindowTitle();
    void                                            detachAllCurves();
    void                                            deletePlotWidget();
    
    void                                            updateAxis(RimDefines::PlotAxis plotAxis);
    std::vector<RimSummaryCurve*>                   curvesForAxis(RimDefines::PlotAxis plotAxis) const;

    void                                            updateTimeAxis();


private:
    caf::PdmField<bool>                             m_showWindow;
    caf::PdmField<QString>                          m_userName;
    caf::PdmChildArrayField<RimSummaryCurve*>       m_curves;
    caf::PdmChildArrayField<RimSummaryCurveFilter*> m_curveFilters;

    caf::PdmField<bool>                             m_isAutoZoom;
    caf::PdmChildField<RimSummaryYAxisProperties*>  m_leftYAxisProperties;
    caf::PdmChildField<RimSummaryYAxisProperties*>  m_rightYAxisProperties;
    caf::PdmChildField<RimSummaryTimeAxisProperties*>  m_timeAxisProperties;

    QPointer<RiuSummaryQwtPlot>                     m_qwtPlot;

    // Internal objects managed by unique_ptr
    std::unique_ptr<RimSummaryYAxisProperties>      m_leftYAxisPropertiesObject;
    std::unique_ptr<RimSummaryYAxisProperties>      m_rightYAxisPropertiesObject;
    std::unique_ptr<RimSummaryTimeAxisProperties>   m_timeAxisPropertiesObject;
};
