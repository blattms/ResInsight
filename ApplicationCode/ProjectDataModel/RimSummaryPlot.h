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

#include <QPointer>

class RiuResultQwtPlot;
class RimSummaryCurve;

//==================================================================================================
///  
///  
//==================================================================================================
class RimSummaryPlot : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    RimSummaryPlot();
    virtual ~RimSummaryPlot();

    QWidget* createPlotWidget(QWidget* parent);
    void deletePlotWidget();

    QWidget* widget();
    RiuResultQwtPlot* qwtPlot();

    void redrawAllCurves();

    caf::PdmChildArrayField<RimSummaryCurve*> summaryCurves;

private:
    QPointer<RiuResultQwtPlot> m_qwtPlot;

};