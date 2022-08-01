/*
Header for evaluation. 

This file declares constants and functions related to evaluation of a WordleSolver
that uses a certain dictionary and ranking scheme.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef EVALUATION_H
#define EVALUATION_H

#include <string_view>
#include <string>
#include <vector>
#include "solver.h"
#include "ranker.h"

// Constant to indicate Evaluate() failed
const short FAILED{-1};

// GridEvaluate() output file suffix
const std::string OUTPUT_SUFFIX{"-output"};

/*
Derives the Wordle feedback for a guess for a particular word.

Parameters:
    guess: A guess. Assumed to be a 5-letter word.
    word: A word. Also assumed to be 5 letters.

Returns:
    The feedback represented as a string of g, b, and y. g represents
    a green square used in the Wordle website, b a black square, and
    y a yellow square.
*/
std::string GetFeedback(std::string_view guess, std::string_view word);

/*
Evaluates a WordleSolver on guessing a word.

Parameters:
    solver: A WordleSolver.
    word: A 5-letter word.

Returns:
    Number of guesses it takes solver to guess word. If the solver fails
    to do so, FAILED is returned.
*/
short Evaluate(WordleSolver &solver, std::string_view word);

/*
Computes various statistics on a WordleSolver's performance.

Parameters:
    guess_counts: Container with guess counts for a WordleSolver on a bunch
    of words.
    mean: Will store the computed mean of the non-FAILED elements of guess_counts.
    std_dev: Will store the computed standard deviation of the non-FAILED elements 
    of guess_counts.
    fail_count: Will store the number of occurrences of FAILED in guess_counts.
*/
void GetStatistics(const std::vector<short> &guess_counts, double &mean, double &std_dev, size_t &fail_count);

/*
Builds and evaluates a collection of WordleSolvers on a list of words.

For each dictionary filepath and ranking scheme, a WordleSolver is created and evaluated
on a list of words using Evaluate(). Statistics from the evaluation are calculated using
GetStatistics() and then are written to a TSV file.

Parameters:
    dictionary_fps: Container of dictionary filepaths. Will be cutoff in output file if
    over 40 characters.
    rankers: Container of ranking schemes specified via AbstractRanker pointers.
    words_fp: Filepath of 5-letter words to evaluate solvers on. Assumed words in this 
    filecan be stored in memory. If words_fp is at path.txt then the output will be
    at path-output.txt. 
*/
void GridEvaluate(const std::vector<std::string> &dictionary_fps, const std::vector<AbstractRanker *> &rankers, const std::string &words_fp);

#endif