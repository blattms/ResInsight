/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA, Ceetron Solutions AS
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

#include "RigCellEdgeResultAccessor.h"

#include <cmath>


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RigCellEdgeResultAccessor::RigCellEdgeResultAccessor(const QString& resultName)
    : m_resultName(resultName)
{
    m_resultAccessObjects.resize(6);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RigCellEdgeResultAccessor::setDataAccessObjectForFace(cvf::StructGridInterface::FaceType faceId, RigResultAccessor* resultAccessObject)
{
    m_resultAccessObjects[faceId] = resultAccessObject;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
double RigCellEdgeResultAccessor::cellScalar(size_t localCellIndex) const
{

    // TODO: How to handle when we get here?
    CVF_ASSERT(false);

    return cvf::UNDEFINED_DOUBLE;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
double RigCellEdgeResultAccessor::cellFaceScalar(size_t localCellIndex, cvf::StructGridInterface::FaceType faceId) const
{
    const RigResultAccessor* resultAccessObj = m_resultAccessObjects.at(faceId);
    if (resultAccessObj != NULL)
    {
        return resultAccessObj->cellFaceScalar(localCellIndex, faceId);
    }

    return HUGE_VAL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString RigCellEdgeResultAccessor::resultName() const
{
    return m_resultName;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RigCellEdgeResultAccessor::setCellScalar(size_t localCellIndex, double scalarValue)
{
    // TODO: How to handle when we get here?
    CVF_ASSERT(false);

}
