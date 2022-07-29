/*
Header for WordleSolver.

This file declares the WordleSolver class which is used to solve the Wordle game
supplied a dictionary of 5 letter words that is a superset of the wordle words
and a ranker specified as a subclass of the abstract class AbstractRanker.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef SOLVER_H
#define SOLVER_H

#include <exception>
#include "ranker.h"
#include <string>
#include <string_view>
#include <unordered_set>

/*
Solver for Wordle game.

Uses a dictionary of 5 letter words and a AbstractRanker to guess
solutions to the Wordle game. Whenever the AbstractRanker is used
to rank words using AbstractRanker::Rank, it is guaranteed that
AbstractRanker::SetUp will be called with the filepath to the most
up to date list of eligible words. Furthermore, it is guaranteed
when run in debug mode that AbstractRanker::GetDebugInfo will be
called after AbstractRanker::SetUp. These conditions make up
a contract between WordleSolver and AbstractRanker that will
allow for the design of derived classes of AbstractRanker.

The WordleSolver solves the game following the hard mode requirements,
that is that words that fail to match the prior feedback are no
longer eligible to be guesses. For instance, the game would never
guess "audio" after receiving the feedback "bgggg" for "creed".
*/
class WordleSolver
{
public:
    /*
    Creates a WordleSolver given a dictionary and ranking scheme.

    By default, the solver runs not in debug mode.

    Parameters:
        d_fp : Path to dictionary file. Dictionary file should have
        a 5-letter word set that is a superset of the wordle words.
        Each word should be on a separate line.

        r : Pointer to AbstractRanker that points to a derived class
        object that implements a ranking scheme.
    */
    WordleSolver(std::string_view d_fp, AbstractRanker *r);

    /*
    Creates a WordleSolver given a dictionary, ranking scheme, and debug mode setting.

    Note, running in debug mode will be noticeably slower and will generate
    a time associated log file. This should be used to analyze the performance
    of a WordleSolver in a highly restricted scenario such as guessing a single
    word. It is the user's responsibility to delete log files.

    Parameters:
        d_fp : Path to dictionary file. Dictionary file should have
        a 5-letter word set that is a superset of the wordle words.
        Each word should be on a separate line.

        r : Pointer to AbstractRanker that points to a derived class
        object that implements a ranking scheme.

        dm : Debug mode.
    */
    WordleSolver(std::string_view d_fp, AbstractRanker *r, bool dm);

    /*
    Returns an initial guess.
    */
    std::string Guess();

    /*
    Makes a guess based on the most recent guess and feedback.

    Parameters:
        feedback: Feedback for the previous output of Guess() in the form of a 5 y's,
        g's, and b's. y represents yellow, g represents green, b represents black.

    Returns:
        The newest guess.
    */
    std::string Guess(std::string_view feedback);

private:
    /*
    Filepath to the dictionary used to make guesses. The dictionary is stored
    in a file versus a container so as to not assume that the dictionary can
    fit in memory.
    */
    std::string dictionary_fp;

    /*
    Pointer to the AbstractRanker used to make guesses. Pointer combined with
    virtual AbstractRanker functions enables dynamic binding.
    */
    AbstractRanker *ranker;

    /*
    Stores the current number of guesses the solver has made. Before any
    guess has been made, num_guesses = 0. Otherwise, it's 1-6. num_guesses + 1
    is passed to AbstractRanker::SetUp and Rank.
    */
    unsigned short num_guesses;

    /*
    Set of the indices (0-4) of the solution that have been found so far.
    */
    std::unordered_set<unsigned short> found_indices;

    /*
    Filepath to where the eligible words from the dictionary will be stored.
    These will be the words that remain after processing feedback. A separate
    file is used as to not overwrite the dictionary filepath. The eligible words
    are stored in a file because initially it may not fit in the memory as it
    starts as the dictionary.
    */
    std::string eligible_fp;

    /*
    Stores the previous guess made by the solver. Starts as the empty string as
    all guesses would be 5 letters.
    */
    std::string prev_guess;

    /*
    Inner class that holds private helper functions of WordleSolver. See
    https://stackoverflow.com/a/28734794.
    */
    class Private;

    /*
    Stores a placeholder for feedback (used in the implementation of Guess()).
    Placeholder of "" is valid as real feedback will never be that.
    */
    static const std::string FEEDBACK_PLACEHOLDER;

    /*
    Stores the suffix appended to the dictionary filepath before the extension
    for the eligible words file.
    */
    static const std::string ELIGIBLE_FP_SUFFIX;

    /*
    Stores the suffix appended to the dictionary filepath before the extension
    and time for the log file.
    */
    static const std::string LOG_FP_SUFFIX;

    // Flag for whether the WordleSolver is running in debug mode.
    bool debug_mode;

    // Debug log filepath
    std::string debug_log_fp;
};

#endif