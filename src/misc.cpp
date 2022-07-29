// Implementation for misc.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "misc.h"

std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix)
{
    auto idx{fp.find_last_of('.')};
    return fp.substr(0, idx) + suffix + fp.substr(idx);
}

WordleSolverException::WordleSolverException(std::string_view r) : reason(r) {}

const char *WordleSolverException::what() const throw()
{
    return reason.c_str();
}
