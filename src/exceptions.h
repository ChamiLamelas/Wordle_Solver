/*
Header for exceptions.

This file holds the definition of WordleSolverException, which
is the only exception type used by the derived classes of
BaseRanker, WordleSolver, and the functions of the evaluation
header.
*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>
#include <string_view>

/*
WordleSolver exception class.

This is a custom exception class that is used to report errors.
This class is created to distinguish errors in BaseRanker,
WordleSolver, and the evaluation header from exceptions raised
by C++.
*/
class WordleSolverException final : public std::exception
{
public:
    /*
    Constructs a WordleSolverException.

    Parameters:
        r: Reason for the exception being thrown.
    */
    WordleSolverException(std::string_view r);

    /*
    Specifies why the exception was thrown to a catch block for example.

    Returns:
        Reason for exception being thrown.

    Throws:
        Nothing, customary to not have any exception method throw an exception.
    */
    const char *what() const throw() override;

private:
    /*
    Stores the reason the exception was thrown, defines the C-string returned
    by what().
    */
    std::string reason;
};

#endif