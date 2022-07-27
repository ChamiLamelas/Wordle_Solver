#ifndef USER_H
#define USER_H

#include "ranker.h"

/*
Runs user mode for user to enter Wordle feedback.

This is the default mode of operation for the main client code. That is,
a WordleSolver is constructed and used with the feedback being typed in
by the user. The user types in b where they see a black square on the
Wordle website, y when they see a yellow square, and g when they
see a green square.

Parameters:
    dictionary_fp: Dictionary filepath.
    ranker: Specifies ranking scheme.
*/
void RunUserMode(std::string_view dictionary_fp, BaseRanker *ranker);

#endif