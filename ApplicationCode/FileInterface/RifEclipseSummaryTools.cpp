/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
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

#include "RifEclipseSummaryTools.h"

#include "RifReaderEclipseSummary.h"

#include "ecl_util.h"

#include <iostream>



//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RifEclipseSummaryTools::findSummaryHeaderFile(const std::string& inputFile, std::string* headerFile, bool* isFormatted)
{
    findSummaryHeaderFileInfo(inputFile, headerFile, NULL, NULL, isFormatted);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::vector<std::string> RifEclipseSummaryTools::findSummaryDataFiles(const std::string& caseFile)
{
    std::vector<std::string> fileNames;

    std::string path;
    std::string base;

    findSummaryHeaderFileInfo(caseFile, NULL, &path, &base, NULL);
    if (path.empty() || base.empty()) return fileNames;

    char* header_file = NULL;
    stringlist_type* summary_file_list = stringlist_alloc_new();

    ecl_util_alloc_summary_files(path.data(), base.data(), NULL, &header_file, summary_file_list);
    if (stringlist_get_size( summary_file_list ) > 0)
    {
        for (int i = 0; i < stringlist_get_size(summary_file_list); i++)
        {
            fileNames.push_back(stringlist_iget(summary_file_list, i));
        }
    }

    util_safe_free(header_file);
    stringlist_free(summary_file_list);

    return fileNames;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RifEclipseSummaryTools::dumpMetaData(RifReaderEclipseSummary* readerEclipseSummary)
{
    std::cout << " -- Well names --" << std::endl;
    {
        std::vector<std::string> names =  readerEclipseSummary->wellNames();

        for (size_t i = 0; i < names.size(); i++)
        {
            std::cout << names[i] << std::endl;
        }
    }

    std::cout << " -- Well variable names --" << std::endl;
    {
        std::vector<std::string> names =  readerEclipseSummary->wellVariableNames();

        for (size_t i = 0; i < names.size(); i++)
        {
            std::cout << names[i] << std::endl;
        }
    }

    std::cout << " -- Group names --" << std::endl;
    {
        std::vector<std::string> names =  readerEclipseSummary->wellGroupNames();

        for (size_t i = 0; i < names.size(); i++)
        {
            std::cout << names[i] << std::endl;
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RifEclipseSummaryTools::findSummaryHeaderFileInfo(const std::string& inputFile, std::string* headerFile, std::string* path, std::string* base, bool* isFormatted)
{
    char* myPath = NULL;
    char* myBase = NULL;
    bool formattedFile = true;

    util_alloc_file_components(inputFile.data(), &myPath, &myBase, NULL);

    char* myHeaderFile = ecl_util_alloc_exfilename(myPath, myBase, ECL_SUMMARY_HEADER_FILE, true, -1);
    if (!myHeaderFile)
    {
        myHeaderFile = ecl_util_alloc_exfilename(myPath, myBase, ECL_SUMMARY_HEADER_FILE, false, -1);
        if (myHeaderFile)
        {
            formattedFile = false;
        }
    }

    if (myHeaderFile && headerFile) *headerFile = myHeaderFile;
    if (myPath && path)             *path = myPath;
    if (myBase && base)             *base = myBase;
    if (isFormatted)                *isFormatted = formattedFile;

    util_safe_free(myHeaderFile);
    util_safe_free(myBase);
    util_safe_free(myPath);
}