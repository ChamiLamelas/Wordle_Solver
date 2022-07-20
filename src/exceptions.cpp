#include "exceptions.h"

WordleSolverException::WordleSolverException(std::string_view r) : reason(r) {}

const char *WordleSolverException::what() const throw()
{
    return reason.c_str();
}
