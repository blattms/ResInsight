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

#include "qwt_plot.h"

#include <QString>

//==================================================================================================
///  
///  
//==================================================================================================
class RimSummaryYAxisProperties : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    enum NumberFormatType
    {
        NUMBER_FORMAT_AUTO,
        NUMBER_FORMAT_DECIMAL,
        NUMBER_FORMAT_SCIENTIFIC
    };

public:
    RimSummaryYAxisProperties();

    void setNameAndAxis(const QString& name, QwtPlot::Axis axis);
    QwtPlot::Axis axis() const;

    caf::PdmField<bool>         isAutoTitle;
    caf::PdmField<QString>      customTitle;
    caf::PdmField<int>          fontSize;

    caf::PdmField<double>       visibleRangeMin;
    caf::PdmField<double>       visibleRangeMax;

    caf::PdmField< caf::AppEnum< NumberFormatType > > numberFormat;

    caf::PdmField<bool>         isLogarithmicScaleEnabled;

    virtual void                            fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue) override;
    virtual caf::PdmFieldHandle*            userDescriptionField() override;
    virtual QList<caf::PdmOptionItemInfo>   calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions, bool * useOptionsOnly) override;

protected:
    virtual void                            initAfterRead() override;

private:
    void                                    updateOptionSensitivity();

private:
    caf::PdmField<QString>      m_name;
    QwtPlot::Axis               m_axis;
};
