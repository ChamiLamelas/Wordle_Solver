// Implementation for exceptions header.
// Author: Chami Lamelas
// Date: Summer 2022

#include "exceptions.h"

WordleSolverException::WordleSolverException(std::string_view r) : reason(r) {}

const char *WordleSolverException::what() const throw()
{
    return reason.c_str();
}
