/*
Header for user mode. 

This file declares the RunUserMode function to allow one to use the solver
while they are trying to solve the Wordle game online.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef USER_H
#define USER_H

#include "ranker.h"

/*
Runs user mode for user to enter Wordle feedback.

That is, a WordleSolver is constructed and used with the feedback being 
typed in by the user. The user types in b where they see a black square 
on the Wordle website, y when they see a yellow square, and g when they
see a green square.

Parameters:
    dictionary_fp: Dictionary filepath.
    ranker: Specifies ranking scheme.
*/
void RunUserMode(std::string_view dictionary_fp, AbstractRanker *ranker);

/*
Overloaded version of RunUserMode that allows for debugging.

This version supplies an additional parameter that allows the underlying
WordleSolver to be run in debug mode to see how the solver is reducing
eligible words and rankers are computing various statistics. By default,
RunUserMode is run not in debug mode as it is slower and clutters the
console output.

Parameters:
    dictionary_fp: Dictionary filepath.
    ranker: Specifies ranking scheme.
    debug_mode: Whether underlying WordleSolver should run in debug mode.
*/
void RunUserMode(std::string_view dictionary_fp, AbstractRanker *ranker, bool debug_mode);

#endif