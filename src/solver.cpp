// Implementation of WordleSolver
// Author: Chami Lamelas
// Date: Summer 2022

#include "solver.h"
#include <fstream>
#include <limits>
#include <iostream>
#include "exceptions.h"

// Helper function that inserts a suffix into a filepath infront of the extension
// Takes const string& for 2 reasons: easier to concatenate strings and filepaths
// tend to be taken as const string& in many C++ functions.
// Example: InsertFilePathSuffix("a.txt", "b") -> "ab.txt"
std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix)
{
    auto idx = fp.find_last_of('.');
    return fp.substr(0, idx) + suffix + fp.substr(idx);
}

class WordleSolver::Private
{

public:
    // reduces the eligible words stored by a WordleSolver based on a binary
    // predicate that can be a function object, function pointer (i.e to
    // an explicit function), or a lambda (see: https://stackoverflow.com/a/2938589)
    template <typename Functor>
    static void KeepOnPredicate(WordleSolver &self, const Functor &predicate)
    {
        // Read-only file stream to eligible words file
        std::ifstream eligible_file_r(self.eligible_fp, std::ios_base::in);
        if (!eligible_file_r.is_open())
        {
            throw WordleSolverException("Could not open " + self.eligible_fp + " for reading");
        }

        // Construct path to temporary file used to reduce eligible words
        std::string temp_fp{InsertFilePathSuffix(self.eligible_fp, ELIGIBLE_FP_SUFFIX)};

        // Open read and write file stream to this file, trunc specifies that this file will be created
        // if it does not exist already
        std::fstream temp_file_rw(temp_fp, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
        if (!temp_file_rw.is_open())
        {
            throw WordleSolverException("Could not create or open and clear " + temp_fp + " for reading and writing");
        }

        // Whether or not writing first word to temp file, used to
        // put \n after all words besides the last one
        bool first_word{true};
        std::string word;

        // Reads eligible words one at a time, writing only the words
        // for which the predicate returns true. Once first word is
        // written, first_word is set to false meaning \n will be
        // written before each following word
        while (eligible_file_r.good())
        {
            std::getline(eligible_file_r, word);
            if (!predicate(word))
            {
                continue;
            }

            if (!first_word)
            {
                temp_file_rw << "\n";
            }

            temp_file_rw << word;
            if (first_word)
            {
                first_word = false;
            }
        }

        // Closing file automatically flushes the buffer
        eligible_file_r.close();

        // Write-only file stream to eligible words file (will overwrite it)
        std::ofstream eligible_file_w(self.eligible_fp, std::ios_base::out);
        if (!eligible_file_w.is_open())
        {
            throw WordleSolverException("Could not open " + self.eligible_fp + " for writing");
        }

        // Resets temp file stream to beginning as we now we will be
        // reading from it. First clear error flags (EOF bit will be set)
        // then set stream to beginning.
        temp_file_rw.clear();
        temp_file_rw.seekg(0);

        // Reset first_word for same purpose as above
        first_word = true;

        // Copy temp file to eligible file line by line
        while (temp_file_rw.good())
        {
            std::getline(temp_file_rw, word);
            if (!first_word)
            {
                eligible_file_w << "\n";
            }

            eligible_file_w << word;
            if (first_word)
            {
                first_word = false;
            }
        }

        temp_file_rw.close();
        eligible_file_w.close();
    }

    // Updates the eligible words stored by a WordleSolver given feedback
    static void UpdateEligibleWords(WordleSolver &self, std::string_view feedback)
    {
        // Set of characters that have been identified as yellow
        std::set<char> yellow;

        // First we will pass over feedback and just work with green feedback in the hope
        // that learning prev_guess[idx] for is meant to be at index idx will clear out
        // a lot of eligible words
        for (auto fitr{feedback.cbegin()}; fitr != feedback.cend(); fitr++)
        {
            auto idx{std::distance(feedback.cbegin(), fitr)};
            // Feedback green at this letter => found correct letter in correct place, if
            // this found only on the previous guess we need to adjust eligible words.
            if (*fitr == 'g' && self.found_indices.find(idx) == self.found_indices.end())
            {
                // Green predicate: Keep only words of eligible words that have prev_guess[idx]
                // at idx
                Private::KeepOnPredicate(self, [&self, &idx](std::string_view word)
                                         { return word[idx] == self.prev_guess[idx]; });

                // Mark that we have found letter at idx, this means in future guesses that
                // are made by the solver, we won't go through eligible words uneccessarily
                // as we have ensured above that all the eligible words in future rounds
                // will only have prev_guess[idx] at idx
                self.found_indices.insert(idx);
            }
        }

        // Next pass over feedback will deal with yellow and black feedback as those will
        // most likely remove less words
        for (auto fitr{feedback.cbegin()}; fitr != feedback.cend(); fitr++)
        {
            auto idx{std::distance(feedback.cbegin(), fitr)};
            if (*fitr == 'y')
            {
                // Mark letter has been found as yellow (used in black predicate)
                yellow.insert(self.prev_guess[idx]);

                // Yellow predicate: keep words that meet all of these conditions
                // 1. prev_guess[idx] is in word
                // 2. prev_guess[idx] is not at idx in word
                // 3. prev_guess[idx] is not at any of the indices that have already been found
                Private::KeepOnPredicate(self, [&self, &idx](std::string_view word)
                                         {
                    auto loc{word.find(self.prev_guess[idx])};
                    return loc != std::string::npos && loc != idx && self.found_indices.find(loc) == self.found_indices.end(); });
            }
            else if (*fitr == 'b')
            {
                Private::KeepOnPredicate(self, [&self, &idx, &yellow](std::string_view word)
                                         {
                // Black predicate: If prev_guess[idx] was seen at an earlier index and was 
                // marked yellow, then only keep words where it has a character other than
                // prev_guess[idx] at idx. Why? If we have feedback where a letter is first
                // marked yellow at index i than later black at index j, that means the letter 
                // occurs in the word at neither i or j, but could be anywhere else. Example:
                // Feedback for guess "three" for solution "beach" is "bybyb".
                if (yellow.find(self.prev_guess[idx]) != yellow.end())
                {
                    return word[idx] != self.prev_guess[idx];
                }
                // If prev_guess[idx] was not seen at an earlier index and marked yellow, then
                // only keep words where prev_guess[idx] is not anywhere in the word other than
                // the found indices. Why? A letter can be marked black at some letter(s) and
                // green at others. Example: Feedback for guess "there" for solution "their"
                // is "gggyb".
                for (auto widx{0}; widx < word.size(); widx++)
                {
                    if (self.found_indices.find(widx) == self.found_indices.end() && word[widx] == self.prev_guess[idx])
                    {
                        return false;
                    }
                }
                return true; });
            }
        }
    }

    // Copies dictionary into eligible words file
    static void CopyDictionary(WordleSolver &self)
    {
        // Read-only file stream for dictionary
        std::ifstream dictionary_file(self.dictionary_fp, std::ios_base::in);
        if (!dictionary_file.is_open())
        {
            throw WordleSolverException("Could not open dictionary file for reading");
        }

        // Write-only file stream for eligible words
        std::ofstream eligible_file(self.eligible_fp, std::ios_base::out);
        if (!eligible_file.is_open())
        {
            throw WordleSolverException("Could not open eligible words file for writing");
        }

        // Read dictionary file line by line and copy each word to eligible words
        // file - first_word used to put \n after all but the last word
        bool first_word{true};
        std::string word;
        while (dictionary_file.good())
        {
            std::getline(dictionary_file, word);
            if (!first_word)
            {
                eligible_file << "\n";
            }

            eligible_file << word;
            if (first_word)
            {
                first_word = false;
            }
        }

        dictionary_file.close();
        eligible_file.close();
    }
};

// Feedback could never be "", also see: https://stackoverflow.com/a/2605559
const std::string WordleSolver::FEEDBACK_PLACEHOLDER = "";
const std::string WordleSolver::ELIGIBLE_FP_SUFFIX = "-temp";

WordleSolver::WordleSolver(std::string_view d_fp, BaseRanker *r) : num_guesses(0), ranker(r), dictionary_fp(d_fp), eligible_fp(InsertFilePathSuffix(dictionary_fp, WordleSolver::ELIGIBLE_FP_SUFFIX)) {}

std::string WordleSolver::Guess()
{
    return Guess(FEEDBACK_PLACEHOLDER);
}

std::string WordleSolver::Guess(std::string_view feedback)
{
    // Making initial guess
    if (feedback == FEEDBACK_PLACEHOLDER)
    {
        // Copy dictionary into eligible words file, reset indices that have been
        // found and that no guesses have been made (yet)
        Private::CopyDictionary(*this);
        found_indices.clear();
        num_guesses = 0;
    }
    else // When we have feedback, use it
    {
        Private::UpdateEligibleWords(*this, feedback);
    }

    // Read-only file stream for eligible words
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open eligible words");
    }

    // Prepare ranker (part of contract between WordleSolver and BaseRanker)
    ranker->SetUp(eligible_fp);

    // word with rank = current_min_rank
    std::string current_guess;

    // lowest rank seen so far (every words rank assumed < INT_MAX)
    int current_min_rank{INT_MAX};

    // rank of current word
    int current_rank;

    // Read over eligible words line by line and calculate rank, update
    // current_min_rank and current_guess accordingly
    std::string word;
    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        current_rank = ranker->Rank(word);
        if (current_rank < current_min_rank)
        {
            current_guess = word;
            current_min_rank = current_rank;
        }
    }
    eligible_file.close();

    // current_guess was never set in above loop
    if (current_guess.empty())
    {
        throw WordleSolverException("Unable to make guess - no eligible words");
    }

    // Mark we have made guess and store guess to make future guesses
    num_guesses++;
    prev_guess = current_guess;
    return current_guess;
}
