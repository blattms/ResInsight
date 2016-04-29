
# Use this workaround until we're on 2.8.3 on all platforms and can use CMAKE_CURRENT_LIST_DIR directly 
if (${CMAKE_VERSION} VERSION_GREATER "2.8.2")
    set(CEE_CURRENT_LIST_DIR  ${CMAKE_CURRENT_LIST_DIR}/)
endif()


set (SOURCE_GROUP_HEADER_FILES
${CEE_CURRENT_LIST_DIR}RicCreateGraphPlotMainWindowFeature.h
${CEE_CURRENT_LIST_DIR}RicDropEnabledMainWindow.h
${CEE_CURRENT_LIST_DIR}RicDeleteSummaryPlotFeature.h
${CEE_CURRENT_LIST_DIR}RicAddSummaryCurveFeature.h
${CEE_CURRENT_LIST_DIR}RicTileGraphWindowsFeature.h
)

set (SOURCE_GROUP_SOURCE_FILES
${CEE_CURRENT_LIST_DIR}RicCreateGraphPlotMainWindowFeature.cpp
${CEE_CURRENT_LIST_DIR}RicDropEnabledMainWindow.cpp
${CEE_CURRENT_LIST_DIR}RicDeleteSummaryPlotFeature.cpp
${CEE_CURRENT_LIST_DIR}RicAddSummaryCurveFeature.cpp
${CEE_CURRENT_LIST_DIR}RicTileGraphWindowsFeature.cpp
)

list(APPEND CODE_HEADER_FILES
${SOURCE_GROUP_HEADER_FILES}
)

list(APPEND CODE_SOURCE_FILES
${SOURCE_GROUP_SOURCE_FILES}
)

list(APPEND QT_MOC_HEADERS
${CEE_CURRENT_LIST_DIR}RicDropEnabledMainWindow.h
)


source_group( "CommandFeature\\GraphPlots" FILES ${SOURCE_GROUP_HEADER_FILES} ${SOURCE_GROUP_SOURCE_FILES} ${CEE_CURRENT_LIST_DIR}CMakeLists_files.cmake )
