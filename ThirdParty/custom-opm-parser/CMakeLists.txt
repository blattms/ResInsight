cmake_minimum_required (VERSION 2.8)


# -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_TEST_DYN_LINK -DHAVE_CASE_SENSITIVE_FILESYSTEM=1 -DHAVE_REGEX=1 -DOPM_PARSER_DECK_API=1 -Wall -std=c++11 -fopenmp -Wall -g   -O0 -DDEBUG  -ggdb3
 
 
 if(MSVC)
  add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4996" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else()
  #always set these flags to make opm-parser compile on Linux
  SET( CMAKE_CXX_FLAGS "-pipe -std=c++0x -pedantic -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Wcast-qual -Wshadow -Wwrite-strings -Wchar-subscripts -Wredundant-decls")

  SET( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}   -O0 -DDEBUG  -ggdb3")
  SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -mtune=native")
endif(MSVC)

project (custom-opm-parser)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${ResInsight_SOURCE_DIR}/ThirdParty/custom-opm-common/opm-common/
    ${ERT_INCLUDE_DIRS}
)



# TODO: opm-parser should hold a cmake file with source code files only
#include(opm-parser/CMakeLists_files.cmake)

set( rawdeck_source 
RawDeck/StarToken.cpp
RawDeck/RawKeyword.cpp 
RawDeck/RawRecord.cpp )

set( unit_source
Units/UnitSystem.cpp
Units/Dimension.cpp)

set( deck_source
Deck/Deck.cpp
Deck/DeckKeyword.cpp
Deck/DeckRecord.cpp
Deck/DeckItem.cpp
Deck/Section.cpp
Deck/SCHEDULESection.cpp
Deck/DeckTimeStep.cpp
)

set( parser_source
Parser/ParseContext.cpp
Parser/MessageContainer.cpp
Parser/ParserEnums.cpp
Parser/ParserKeyword.cpp 
Parser/Parser.cpp 
Parser/ParserRecord.cpp
Parser/ParserItem.cpp
Parser/ParserIntItem.cpp  
Parser/ParserDoubleItem.cpp
Parser/ParserStringItem.cpp
)

set( generator_source
Generator/KeywordGenerator.cpp
Generator/KeywordLoader.cpp )

set( build_parser_source 
Parser/ParseContext.cpp
Parser/MessageContainer.cpp
Parser/ParserEnums.cpp
Parser/ParserKeyword.cpp 
Parser/ParserRecord.cpp
Parser/ParserItem.cpp
Parser/ParserIntItem.cpp  
Parser/ParserDoubleItem.cpp
Parser/ParserStringItem.cpp  
${generator_source}
)

set (state_source
EclipseState/EclipseState.cpp
EclipseState/EclipseConfig.cpp
EclipseState/Eclipse3DProperties.cpp
EclipseState/Messages.cpp
#
EclipseState/checkDeck.cpp
#
EclipseState/Schedule/OilVaporizationProperties.cpp
EclipseState/Schedule/TimeMap.cpp 
EclipseState/Schedule/Schedule.cpp 
EclipseState/Schedule/Well.cpp
EclipseState/Schedule/WellProductionProperties.cpp
EclipseState/Schedule/WellInjectionProperties.cpp
EclipseState/Schedule/WellPolymerProperties.cpp
EclipseState/Schedule/MSW/Segment.cpp
EclipseState/Schedule/MSW/SegmentSet.cpp
EclipseState/Schedule/MSW/Compsegs.cpp
EclipseState/Schedule/WellSet.cpp
EclipseState/Schedule/Group.cpp
EclipseState/Schedule/Completion.cpp
EclipseState/Schedule/CompletionSet.cpp
EclipseState/Schedule/ScheduleEnums.cpp 
EclipseState/Schedule/GroupTreeNode.cpp
EclipseState/Schedule/GroupTree.cpp
EclipseState/Schedule/Tuning.cpp
EclipseState/Schedule/Events.cpp
#
EclipseState/Tables/SimpleTable.cpp
EclipseState/Tables/VFPProdTable.cpp
EclipseState/Tables/VFPInjTable.cpp
EclipseState/Tables/TableManager.cpp
EclipseState/Tables/TableContainer.cpp
EclipseState/Tables/TableColumn.cpp
EclipseState/Tables/ColumnSchema.cpp
EclipseState/Tables/TableSchema.cpp
EclipseState/Tables/TableIndex.cpp
EclipseState/Tables/PvtxTable.cpp
EclipseState/Tables/Tables.cpp
#
EclipseState/Grid/SatfuncPropertyInitializers.cpp
EclipseState/Grid/GridProperty.cpp
EclipseState/Grid/GridProperties.cpp
EclipseState/Grid/Box.cpp
EclipseState/Grid/BoxManager.cpp
EclipseState/Grid/FaceDir.cpp
EclipseState/Grid/TransMult.cpp        
EclipseState/Grid/MULTREGTScanner.cpp        
EclipseState/Grid/EclipseGrid.cpp
EclipseState/Grid/FaultFace.cpp
EclipseState/Grid/Fault.cpp
EclipseState/Grid/FaultCollection.cpp
EclipseState/Grid/NNC.cpp
EclipseState/Grid/PinchMode.cpp
#
EclipseState/InitConfig/InitConfig.cpp
EclipseState/InitConfig/Equil.cpp
EclipseState/SimulationConfig/SimulationConfig.cpp
EclipseState/SimulationConfig/ThresholdPressure.cpp
EclipseState/SummaryConfig/SummaryConfig.cpp
EclipseState/IOConfig/IOConfig.cpp)
#

set( utility_source
Utility/Functional.cpp
Utility/Stringview.cpp
)

set( HEADER_FILES
RawDeck/RawConsts.hpp 
RawDeck/RawKeyword.hpp 
RawDeck/RawRecord.hpp 
RawDeck/StarToken.hpp
RawDeck/RawEnums.hpp
#
Deck/Deck.hpp
Deck/DeckKeyword.hpp
Deck/DeckRecord.hpp
Deck/DeckItem.hpp
Deck/Section.hpp
Deck/SCHEDULESection.hpp
Deck/DeckTimeStep.hpp
#
Parser/ParserEnums.hpp
Parser/ParserKeyword.hpp 
Parser/Parser.hpp 
Parser/ParserRecord.hpp
Parser/ParserItem.hpp
Parser/ParserIntItem.hpp  
Parser/ParserDoubleItem.hpp
Parser/ParserStringItem.hpp
Parser/InputErrorAction.hpp
Parser/ParseContext.hpp
Parser/MessageContainer.hpp
#
Generator/KeywordLoader.hpp
Generator/KeywordGenerator.hpp
#
Units/UnitSystem.hpp
Units/Dimension.hpp
Units/ConversionFactors.hpp
#
EclipseState/EclipseState.hpp
EclipseState/EclipseConfig.hpp
EclipseState/Eclipse3DProperties.hpp
EclipseState/Messages.hpp
#
EclipseState/checkDeck.hpp
#
EclipseState/Schedule/OilVaporizationProperties.hpp
EclipseState/Schedule/TimeMap.hpp
EclipseState/Schedule/Schedule.hpp
EclipseState/Schedule/Well.hpp
EclipseState/Schedule/WellProductionProperties.hpp
EclipseState/Schedule/WellInjectionProperties.hpp
EclipseState/Schedule/WellPolymerProperties.hpp
EclipseState/Schedule/MSW/Segment.hpp
EclipseState/Schedule/MSW/SegmentSet.hpp
EclipseState/Schedule/MSW/Compsegs.hpp
EclipseState/Schedule/WellSet.hpp
EclipseState/Schedule/Group.hpp
EclipseState/Schedule/DynamicState.hpp
EclipseState/Schedule/DynamicVector.hpp
EclipseState/Schedule/Completion.hpp
EclipseState/Schedule/CompletionSet.hpp
EclipseState/Schedule/ScheduleEnums.hpp
EclipseState/Schedule/GroupTreeNode.hpp 
EclipseState/Schedule/GroupTree.hpp
EclipseState/Schedule/Tuning.hpp
EclipseState/Schedule/Events.hpp
#
EclipseState/Util/RecordVector.hpp 
EclipseState/Util/OrderedMap.hpp 
EclipseState/Util/Value.hpp 
#
EclipseState/Grid/EclipseGrid.hpp
EclipseState/Grid/GridProperty.hpp
EclipseState/Grid/GridProperties.hpp
EclipseState/Grid/SatfuncPropertyInitializers.hpp
EclipseState/Grid/Box.hpp
EclipseState/Grid/BoxManager.hpp
EclipseState/Grid/FaceDir.hpp
EclipseState/Grid/MinpvMode.hpp
EclipseState/Grid/PinchMode.hpp
EclipseState/Grid/MULTREGTScanner.hpp        
EclipseState/Grid/TransMult.hpp       
EclipseState/Grid/FaultFace.hpp
EclipseState/Grid/Fault.hpp
EclipseState/Grid/FaultCollection.hpp
EclipseState/Grid/NNC.hpp
#
EclipseState/InitConfig/InitConfig.hpp
EclipseState/InitConfig/Equil.hpp
EclipseState/SimulationConfig/SimulationConfig.hpp
EclipseState/SimulationConfig/ThresholdPressure.hpp
EclipseState/SummaryConfig/SummaryConfig.hpp
EclipseState/IOConfig/IOConfig.hpp
#
EclipseState/Tables/Tabdims.hpp
EclipseState/Tables/Eqldims.hpp
EclipseState/Tables/Regdims.hpp
EclipseState/Tables/PlyadsTable.hpp
EclipseState/Tables/PvtoTable.hpp
EclipseState/Tables/RocktabTable.hpp
EclipseState/Tables/PvdoTable.hpp
EclipseState/Tables/PvdgTable.hpp
EclipseState/Tables/PvdsTable.hpp
EclipseState/Tables/SimpleTable.hpp
EclipseState/Tables/PlymaxTable.hpp
EclipseState/Tables/PlyrockTable.hpp
EclipseState/Tables/SwofTable.hpp
EclipseState/Tables/SgwfnTable.hpp
EclipseState/Tables/SwfnTable.hpp
EclipseState/Tables/SgfnTable.hpp
EclipseState/Tables/SsfnTable.hpp
EclipseState/Tables/Sof2Table.hpp
EclipseState/Tables/Sof3Table.hpp
EclipseState/Tables/EnptvdTable.hpp
EclipseState/Tables/PlyviscTable.hpp
EclipseState/Tables/PlydhflfTable.hpp
EclipseState/Tables/PlyshlogTable.hpp
EclipseState/Tables/EnkrvdTable.hpp
EclipseState/Tables/ImkrvdTable.hpp
EclipseState/Tables/SgofTable.hpp
EclipseState/Tables/SlgofTable.hpp
EclipseState/Tables/PvtxTable.hpp
EclipseState/Tables/ImptvdTable.hpp
EclipseState/Tables/RsvdTable.hpp
EclipseState/Tables/RvvdTable.hpp
EclipseState/Tables/RtempvdTable.hpp
EclipseState/Tables/OilvisctTable.hpp
EclipseState/Tables/GasvisctTable.hpp
EclipseState/Tables/WatvisctTable.hpp
EclipseState/Tables/PvtgTable.hpp
EclipseState/Tables/VFPProdTable.hpp
EclipseState/Tables/VFPInjTable.hpp
EclipseState/Tables/TableManager.hpp
EclipseState/Tables/TableContainer.hpp
EclipseState/Tables/SorwmisTable.hpp
EclipseState/Tables/SgcwmisTable.hpp
EclipseState/Tables/MiscTable.hpp
EclipseState/Tables/PmiscTable.hpp
EclipseState/Tables/TlpmixpaTable.hpp
EclipseState/Tables/MsfnTable.hpp
EclipseState/Tables/TableColumn.hpp
EclipseState/Tables/ColumnSchema.hpp
EclipseState/Tables/TableEnums.hpp
EclipseState/Tables/TableSchema.hpp
EclipseState/Tables/TableIndex.hpp
#
Utility/Functional.hpp
Utility/Stringview.hpp)




include_directories(
    opm-parser
    generated-source/include
)

set(opm_parser_source_files 
    ${generated_source} 
    ${state_source} 
    ${rawdeck_source} 
    ${parser_source} 
    ${deck_source} 
    ${unit_source} 
    ${generator_source} 
    ${utility_source}
)

foreach (file ${opm_parser_source_files} )
    list(APPEND incl_path "opm-parser/opm/parser/eclipse/${file}" )
endforeach()


set(opm_parser_generated_source_files 
    generated-source/ParserKeywords.cpp
generated-source/ParserKeywords0.cpp
generated-source/ParserKeywords1.cpp
generated-source/ParserKeywords2.cpp
generated-source/ParserKeywords3.cpp
)

# JSON files included directly, not as a separate lib as in original build configuration 
set (json_source
    opm-parser/opm/json/JsonObject.cpp
    opm-parser/opm/json/cjson/cJSON.c
)

add_library(${PROJECT_NAME} 
    STATIC
    ${incl_path} 
    ${json_source}
    ${opm_parser_generated_source_files} 
)

