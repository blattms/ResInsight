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

#include "RigResultAccessor2d.h"
#include "RigResultAccessor.h"

//==================================================================================================
/// 
//==================================================================================================
class RigTernaryResultAccessor : public RigResultAccessor2d
{
public:
	RigTernaryResultAccessor();

	/// Requires two of the arguments to be present
	void setTernaryResultAccessors(RigResultAccessor* soil, RigResultAccessor* sgas, RigResultAccessor* swat);

	/// Returns [SOil, SWat] regardless of which one of the three is missing. if Soil or SWat is missing, it is calculated 
	/// based on the two others
	virtual cvf::Vec2d cellScalar(size_t gridLocalCellIndex) const;
	virtual cvf::Vec2d cellFaceScalar(size_t gridLocalCellIndex, cvf::StructGridInterface::FaceType faceId) const;

private:
	cvf::ref<RigResultAccessor> m_soilAccessor;
	cvf::ref<RigResultAccessor> m_sgasAccessor;
	cvf::ref<RigResultAccessor> m_swatAccessor;
};
