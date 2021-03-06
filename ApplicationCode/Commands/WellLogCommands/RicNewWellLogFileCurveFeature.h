/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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

#include "cafCmdFeature.h"

#include <vector>

class RimWellLogTrack;
class RimWellLogFileCurve;
class RimWellPath;
class RimWellLogFileChannel;


//==================================================================================================
/// 
//==================================================================================================
class RicNewWellLogFileCurveFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    static RimWellLogFileCurve* addCurve(RimWellLogTrack* plotTrack);

    static void addWellLogChannelsToPlotTrack(RimWellLogTrack* plotTrack, const std::vector<RimWellLogFileChannel*>& wellLogFileChannels);

protected:
    // Overrides
    virtual bool isCommandEnabled();
    virtual void onActionTriggered( bool isChecked );
    virtual void setupActionLook( QAction* actionToSetup );

private:
   RimWellLogTrack* selectedWellLogPlotTrack() const;
   RimWellPath*         selectedWellPathWithLogFile() const;
   bool                 wellLogFilesAvailable() const;
};
