#include <iostream>
#include <string>
#include <string_view>
#include "ranker.h"
#include <algorithm>
#include "random_ranker.h"
#include "solver.h"
#include "evaluation.h"
#include "exceptions.h"

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

/*
Main function - by default uses RunUserMode, can alternatively make
use of evaluation header.
*/
int main()
{
    RandomRanker rr(1);
    BaseRanker *rr_ptr{&rr};
    try
    {
        RunUserMode("data/tabatkins_github_words.txt", rr_ptr);
    }
    catch (const WordleSolverException &e)
    {
        std::cout << "Guess failure: " << e.what() << std::endl;
    }

    return 0;
}


// Private helper functions for RunUserMode

void Trim(std::string &s)
{
    auto first_char{s.find_first_not_of(" ")};
    auto last_char{s.find_last_not_of(" ")};
    s.erase(0, first_char);
    s.erase(last_char + 1);
}

void Lower(std::string &s)
{
    transform(s.cbegin(), s.cend(), s.begin(), tolower);
}

std::string ReadFeedback()
{
    std::string feedback;
    bool reading{true};
    do
    {
        std::cout << "Enter feedback from Wordle: " << std::flush;
        std::getline(std::cin, feedback);
        Trim(feedback);
        if (feedback.size() != 5)
        {
            std::cout << "Invalid length feedback, feedback: " << feedback << "\n";
            continue;
        }
        Lower(feedback);
        if (feedback.find_first_not_of("gby") != std::string::npos)
        {
            std::cout << "Invalid characters in feedback, lowercased feedback: " << feedback << "\n";
        }
        else
        {
            reading = false;
        }
    } while (reading);
    return feedback;
}

void RunUserMode(std::string_view dictionary_fp, BaseRanker *ranker)
{
    WordleSolver solver(dictionary_fp, ranker);
    std::string guess{solver.Guess()};
    std::string feedback;
    for (auto num_attempts{1}; num_attempts <= 6; num_attempts++)
    {
        std::cout << "Guess: " << guess << std::endl;
        feedback = ReadFeedback();
        if (feedback == "ggggg")
        {
            std::cout << "Solver guessed  [" << guess << "] in " << num_attempts << " attempts." << std::endl;
            return;
        }
        guess = solver.Guess(feedback);
    }
    std::cout << "Solver failed to guess the word." << std::endl;
}