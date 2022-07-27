// Implementation of file operations header
// Author: Chami Lamelas
// Date: Summer 2022

#include "file_ops.h"

std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix)
{
    auto idx{fp.find_last_of('.')};
    return fp.substr(0, idx) + suffix + fp.substr(idx);
}