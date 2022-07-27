/*
Header for file related operations. 

This file declares the InsertFilePathSuffix function for use in various other
headers.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <string>

/*
Function that inserts a suffix into a filepath infront of the extension.

Takes const string& over string_view for 2 reasons: easier to concatenate 
strings and filepaths tend to be taken as const string& in many C++ 
functions so a string would be needed ahead of time anyway.

Parameters:
    fp: Filepath
    suffix: Suffix

Returns:
    Filepath with suffix infront of the extension. Example: 
    InsertFilePathSuffix("a.txt", "b") -> "ab.txt"
*/
std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix);

#endif 