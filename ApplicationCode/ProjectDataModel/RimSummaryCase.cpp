/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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

#include "RimSummaryCase.h"

#include "RigSummaryCaseData.h"
#include "RimEclipseCase.h"
#include "RimSummaryCaseCollection.h"

#include <QFileInfo>
#include "RimProject.h"
#include "RimSummaryPlotCollection.h"
#include "RimMainPlotCollection.h"

CAF_PDM_ABSTRACT_SOURCE_INIT(RimSummaryCase,"SummaryCase");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryCase::RimSummaryCase()
{
    CAF_PDM_InitObject("Summary Case",":/Cases16x16.png","","");

    CAF_PDM_InitField(&curveDisplayName, "CurveDisplayName", QString("Curve Display Name"), "Curve Display Name", "", "", "");
    CAF_PDM_InitField(&autoCurveDisplayName, "AutoCurveDisplayName", true, "Auto Curve Display Name", "", "", "");
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimSummaryCase::~RimSummaryCase()
{

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryCase::loadCase()
{
    if (m_summaryCaseData.isNull()) m_summaryCaseData = new RigSummaryCaseData(this->summaryHeaderFilename());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString RimSummaryCase::caseName()
{
    QFileInfo caseFileName(this->summaryHeaderFilename());

    return caseFileName.completeBaseName();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryCase::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue)
{
    if (changedField == &autoCurveDisplayName)
    {
        if (autoCurveDisplayName)
        {
            RimSummaryCaseCollection* summaryCaseCollection = NULL;
            this->firstAnchestorOrThisOfType(summaryCaseCollection);
            if (summaryCaseCollection)
            {
                curveDisplayName = summaryCaseCollection->uniqueShortNameForCase(this);
            }
        }
    }

    RimProject* proj = NULL;
    this->firstAnchestorOrThisOfType(proj);
    
    RimMainPlotCollection* mainPlotColl = proj->mainPlotCollection();
    RimSummaryPlotCollection* summaryPlotColl = mainPlotColl->summaryPlotCollection();

    summaryPlotColl->updateSummaryNameHasChanged();

    updateOptionSensitivity();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimSummaryCase::updateOptionSensitivity()
{
    curveDisplayName.uiCapability()->setUiReadOnly(autoCurveDisplayName);
}