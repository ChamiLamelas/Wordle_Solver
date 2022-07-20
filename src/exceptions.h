#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>
#include <string_view>

class WordleSolverException : public std::exception
{
private:
    std::string reason;

public:
    WordleSolverException(std::string_view r);
    virtual const char *what() const throw();
};

#endif