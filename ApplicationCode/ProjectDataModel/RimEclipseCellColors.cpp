/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#include "RimEclipseCellColors.h"

#include "RimCellEdgeColors.h"
#include "RimEclipseFaultColors.h"
#include "RimEclipseView.h"
#include "RimLegendConfig.h"
#include "RimTernaryLegendConfig.h"
#include "RimViewController.h"
#include "RimViewLinker.h"

#include "RiuMainWindow.h"

#include "cafPdmUiTreeOrdering.h"

CAF_PDM_SOURCE_INIT(RimEclipseCellColors, "ResultSlot");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimEclipseCellColors::RimEclipseCellColors()
{
    CAF_PDM_InitObject("Cell Result", ":/CellResult.png", "", "");

    CAF_PDM_InitFieldNoDefault(&obsoleteField_legendConfig, "LegendDefinition", "Legend Definition", "", "", "");
    this->obsoleteField_legendConfig.xmlCapability()->setIOWritable(false);

    CAF_PDM_InitFieldNoDefault(&m_legendConfigData, "ResultVarLegendDefinitionList", "", "", "", "");

    CAF_PDM_InitFieldNoDefault(&ternaryLegendConfig, "TernaryLegendDefinition", "Ternary Legend Definition", "", "", "");
    this->ternaryLegendConfig = new RimTernaryLegendConfig();

    CAF_PDM_InitFieldNoDefault(&m_legendConfigPtrField, "LegendDefinitionPtrField", "Legend Definition PtrField", "", "", "");

    // Make sure we have a created legend for the default/undefined result variable
    changeLegendConfig(this->resultVariable());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimEclipseCellColors::~RimEclipseCellColors()
{
    CVF_ASSERT(obsoleteField_legendConfig() == NULL);

    m_legendConfigData.deleteAllChildObjects();

    delete ternaryLegendConfig();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue)
{
    RimEclipseResultDefinition::fieldChangedByUi(changedField, oldValue, newValue);

    // Update of legend config must happen after RimEclipseResultDefinition::fieldChangedByUi(), as this function modifies this->resultVariable()
    if (changedField == &m_resultVariableUiField)
    {
        if (oldValue != newValue)
        {
            changeLegendConfig(this->resultVariable());
        }

        if (newValue != RimDefines::undefinedResultName())
        {
            if (m_reservoirView) m_reservoirView->hasUserRequestedAnimation = true;
        }

        RimEclipseFaultColors* faultColors = dynamic_cast<RimEclipseFaultColors*>(this->parentField()->ownerObject());
        if (faultColors)
        {
            faultColors->updateConnectedEditors();
        }

        RimCellEdgeColors* cellEdgeColors = dynamic_cast<RimCellEdgeColors*>(this->parentField()->ownerObject());
        if (cellEdgeColors)
        {
            cellEdgeColors->updateConnectedEditors();
        }
    }

    if (m_reservoirView) m_reservoirView->createDisplayModelAndRedraw();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::changeLegendConfig(QString resultVarNameOfNewLegend)
{
    if (resultVarNameOfNewLegend != RimDefines::ternarySaturationResultName())
    {
        bool found = false;

        QString legendResultVariable;

        if (this->m_legendConfigPtrField())
        {
            legendResultVariable = this->m_legendConfigPtrField()->resultVariableName();
        }

        if (!this->m_legendConfigPtrField() || legendResultVariable != resultVarNameOfNewLegend)
        {
            for (size_t i = 0; i < m_legendConfigData.size(); i++)
            {
                if (m_legendConfigData[i]->resultVariableName() == resultVarNameOfNewLegend)
                {
                    this->m_legendConfigPtrField = m_legendConfigData[i];
                    found = true;
                    break;
                }
            }

            // Not found ?
            if (!found)
            {
                    RimLegendConfig* newLegend = new RimLegendConfig;
                    newLegend->setReservoirView(m_reservoirView);
                    newLegend->resultVariableName = resultVarNameOfNewLegend;
                    m_legendConfigData.push_back(newLegend);

                    this->m_legendConfigPtrField = newLegend;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::initAfterRead()
{
    RimEclipseResultDefinition::initAfterRead();

    if (this->m_legendConfigPtrField() && this->m_legendConfigPtrField()->resultVariableName == "")
    {
        this->m_legendConfigPtrField()->resultVariableName = this->resultVariable();
    }

    if (obsoleteField_legendConfig)
    {
        // The current legend config is NOT stored in <ResultVarLegendDefinitionList> in ResInsight up to v 1.3.7-dev
        RimLegendConfig* obsoleteLegend = obsoleteField_legendConfig();

        // set to NULL before pushing into container
        obsoleteField_legendConfig = NULL;

        m_legendConfigData.push_back(obsoleteLegend);
        m_legendConfigPtrField = obsoleteLegend;
    }

    changeLegendConfig(this->resultVariable());

    updateIconState();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::defineUiTreeOrdering(caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName /*= ""*/)
{
    if (this->resultVariable() == RimDefines::ternarySaturationResultName())
    {
        uiTreeOrdering.add(ternaryLegendConfig());
    }
    else
    {
        uiTreeOrdering.add(m_legendConfigPtrField());
    }

   uiTreeOrdering.setForgetRemainingFields(true);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::updateLegendCategorySettings()
{
    changeLegendConfig(this->resultVariable());

    if (this->hasCategoryResult())
    {
        legendConfig()->setMappingMode(RimLegendConfig::CATEGORY_INTEGER);
        legendConfig()->setColorRangeMode(RimLegendConfig::CATEGORY);
    }
    else
    {
        if (legendConfig()->mappingMode() == RimLegendConfig::CATEGORY_INTEGER)
        {
            legendConfig()->setMappingMode(RimLegendConfig::LINEAR_CONTINUOUS);
        }

        if (legendConfig()->colorRangeMode() == RimLegendConfig::CATEGORY)
        {
            legendConfig()->setColorRangeMode(RimLegendConfig::NORMAL);
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::setReservoirView(RimEclipseView* ownerReservoirView)
{
    this->setEclipseCase(ownerReservoirView->eclipseCase());

    m_reservoirView = ownerReservoirView;

    for (size_t i = 0; i < m_legendConfigData.size(); i++)
    {
        m_legendConfigData[i]->setReservoirView(ownerReservoirView);
    }

    this->ternaryLegendConfig()->setReservoirView(ownerReservoirView);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimEclipseView* RimEclipseCellColors::reservoirView()
{
    return m_reservoirView;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::setResultVariable(const QString& val)
{
    RimEclipseResultDefinition::setResultVariable(val);

    this->changeLegendConfig(val);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RimLegendConfig* RimEclipseCellColors::legendConfig()
{
    return m_legendConfigPtrField;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RimEclipseCellColors::updateIconState()
{
    RimViewController* viewController = m_reservoirView->viewController();
    if (viewController && viewController->isResultColorControlled())
    {
        updateUiIconFromState(false);
    }
    else
    {
        updateUiIconFromState(true);
    }

    uiCapability()->updateConnectedEditors();
}

