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

#pragma once

#pragma once

#include "cvfBase.h"
#include "cvfObject.h"
#include "cvfVector2.h"
#include "cvfStructGrid.h"

//==================================================================================================
/// 
//==================================================================================================
class RigResultAccessor2d : public cvf::Object
{
public:
    virtual cvf::Vec2d cellScalar(size_t gridLocalCellIndex) const = 0;
    virtual cvf::Vec2d cellFaceScalar(size_t gridLocalCellIndex, cvf::StructGridInterface::FaceType faceId) const = 0;
};