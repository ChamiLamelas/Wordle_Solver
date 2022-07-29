// Implementation of user header
// Author: Chami Lamelas
// Date: Summer 2022

#include "user.h"
#include <algorithm>
#include <iostream>
#include "solver.h"

// Removes whitespaces from both ends of a string in place
void Trim(std::string &s)
{
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
}

// Converts string to lowercase in place
void Lower(std::string &s)
{
    transform(s.cbegin(), s.cend(), s.begin(), tolower);
}

// Keeps prompting the user for valid feedback
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

void RunUserMode(std::string_view dictionary_fp, AbstractRanker *ranker)
{
    RunUserMode(dictionary_fp, ranker, false);
}

void RunUserMode(std::string_view dictionary_fp, AbstractRanker *ranker, bool debug_mode)
{
    WordleSolver solver(dictionary_fp, ranker, debug_mode);
    std::string guess;
    std::string feedback;
    for (auto num_attempts{0}; num_attempts < 6; num_attempts++)
    {
        guess = (num_attempts == 0) ? solver.Guess() : solver.Guess(feedback);
        std::cout << "Guess: " << guess << std::endl;
        feedback = ReadFeedback();
        if (feedback == "ggggg")
        {
            std::cout << "Solver guessed \"" << guess << "\" in " << num_attempts + 1 << " attempts." << std::endl;
            return;
        }
    }
    std::cout << "Solver failed to guess the word." << std::endl;
}