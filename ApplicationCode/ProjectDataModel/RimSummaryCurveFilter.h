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

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPointer.h"
#include "cafPdmPtrField.h"
#include "cafPdmChildField.h"
#include "cafPdmChildArrayField.h"

#include "RimPlotCurve.h"
#include "RifEclipseSummaryAddress.h"
#include "cafAppEnum.h"

#include "RimSummaryCurve.h"
#include "RimSummaryFilter.h"
#include "cafPdmPtrArrayField.h"

class RimSummaryCase;
class RifReaderEclipseSummary;
class RiuLineSegmentQwtPlotCurve;

Q_DECLARE_METATYPE(RifEclipseSummaryAddress);

//==================================================================================================
///  
///  
//==================================================================================================
class RimSummaryCurveFilter : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:
    RimSummaryCurveFilter();
    virtual ~RimSummaryCurveFilter();

    void                                    loadDataAndUpdate();
    void                                    setParentQwtPlot(QwtPlot* plot);
    void                                    detachQwtCurves();

private:
    void                                    syncCurvesFromUiSelection();
    void                                    syncUiSelectionFromCurves();

    // Overridden PDM methods
    virtual void                            fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue);
    virtual QList<caf::PdmOptionItemInfo>   calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions, bool* useOptionsOnly);
    virtual void                            defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering) override;
    void                                    defineEditorAttribute(const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute * attribute) override;

    QPointer<QwtPlot>                       m_parentQwtPlot;

    // Fields
    caf::PdmPtrArrayField<RimSummaryCase*>    m_selectedSummaryCases;
    caf::PdmChildArrayField<RimSummaryCurve*> m_curves;

    caf::PdmField<QString>                  m_selectedVariableDisplayField;

    // Filter fields
    caf::PdmChildField<RimSummaryFilter*>   m_summaryFilter;
    caf::PdmField<std::vector<RifEclipseSummaryAddress> > 
                                            m_uiFilterResultMultiSelection;

    caf::PdmField<bool>                     m_applyButtonField;
};
