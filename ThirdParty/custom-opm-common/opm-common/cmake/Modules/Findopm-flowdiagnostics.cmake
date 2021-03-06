# - Find OPM Flow Diagnostics Library
#
# Defines the following variables:
#   opm-flowdiagnostics_INCLUDE_DIRS    Directory of header files
#   opm-flowdiagnostics_LIBRARIES       Directory of shared object files
#   opm-flowdiagnostics_DEFINITIONS     Defines that must be set to compile
#   opm-flowdiagnostics_CONFIG_VARS     List of defines that should be in config.h
#   HAVE_OPM_FLOWDIAGNOSTICS            Binary value to use in config.h

# Copyright (C) 2012 Uni Research AS
# This code is licensed under The GNU General Public License v3.0

# use the generic find routine
include (opm-flowdiagnostics-prereqs)

include (OpmPackage)

find_opm_package (
  # module name
  "opm-flowdiagnostics"

  # dependencies
  "${opm-flowdiagnostics_DEPS}"
  
  # header to search for
  "opm/flowdiagnostics/reorder/tarjan.h"

  # library to search for
  "opmflowdiagnostics"

  # defines to be added to compilations
  ""

  # test program
"#include <opm/flowdiagnostics/reorder/tarjan.h>
int main() {
  const int ia[]  = { 0, 0, 1, 2, 4 };
  const int ja[]  = { 0, 0, 1, 2 };
  int vert[4]     = { 0 };
  int comp[4 + 1] = { 0 };
  int ncomp       =   0  ;
  int work[3 * 4] = { 0 };

  tarjan(4, ia, ja, vert, comp, &ncomp, work);
}
"
  # config variables
  "${opm-flowdiagnostics_CONFIG_VAR}"
  )
