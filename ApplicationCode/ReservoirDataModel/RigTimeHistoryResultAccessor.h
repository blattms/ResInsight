/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
//  Copyright (C) Ceetron Solutions AS
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

#include "cvfStructGrid.h"
#include "RifReaderInterface.h"

class RigCaseData;


class RigTimeHistoryResultAccessor
{
public:
    RigTimeHistoryResultAccessor(RigCaseData* eclipseCaseData, size_t gridIndex, size_t cellIndex, size_t scalarResultIndex, RifReaderInterface::PorosityModelResultType porosityModel);
    void setFace(cvf::StructGridInterface::FaceType face);

    QString             topologyText() const;
    std::vector<double> timeHistoryValues() const;

private:
    void                computeTimeHistoryData();

private:
    RigCaseData*    m_eclipseCaseData;

    size_t          m_gridIndex;
    size_t          m_cellIndex;
    size_t          m_scalarResultIndex;

    cvf::StructGridInterface::FaceType m_face;
    RifReaderInterface::PorosityModelResultType m_porosityModel;

    std::vector<double> m_timeHistoryValues;
};

