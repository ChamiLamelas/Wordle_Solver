// Implementation of WordleSolver

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
        std::set<char> yellow;
        for (auto fitr{feedback.cbegin()}; fitr != feedback.cend(); fitr++)
        {
            auto idx{std::distance(feedback.cbegin(), fitr)};
            auto green_predicate = [&self, &idx](std::string_view word)
            {
                return word[idx] == self.prev_guess[idx];
            };
            if (*fitr == 'g' && self.found_indices.find(idx) == self.found_indices.end())
            {
                Private::KeepOnPredicate(self, green_predicate);
                self.found_indices.insert(idx);
                yellow.erase(self.prev_guess[idx]);
            }
        }

        for (auto fitr{feedback.cbegin()}; fitr != feedback.cend(); fitr++)
        {
            auto idx{std::distance(feedback.cbegin(), fitr)};
            auto yellow_predicate = [&self, &idx](std::string_view word)
            {
                auto loc{word.find(self.prev_guess[idx])};
                return loc != std::string::npos && loc != idx && self.found_indices.find(loc) == self.found_indices.end();
            };
            auto black_predicate = [&self, &idx, &yellow](std::string_view word)
            {
                if (yellow.find(self.prev_guess[idx]) != yellow.end())
                {
                    return word[idx] != self.prev_guess[idx];
                }
                for (auto widx{0}; widx < word.size(); widx++)
                {
                    if (self.found_indices.find(widx) == self.found_indices.end() && word[widx] == self.prev_guess[idx])
                    {
                        return false;
                    }
                }
                return true;
            };
            if (*fitr == 'y')
            {
                yellow.insert(self.prev_guess[idx]);
                Private::KeepOnPredicate(self, yellow_predicate);
            }
            else if (*fitr == 'b')
            {
                Private::KeepOnPredicate(self, black_predicate);
            }
        }
    }

    static void CopyDataset(WordleSolver &self)
    {
        std::ifstream dataset_file(self.dictionary_fp, std::ios_base::in);
        if (!dataset_file.is_open())
        {
            throw WordleSolverException("Could not open dataset for reading");
        }

        std::ofstream temp_file(self.eligible_fp, std::ios_base::out);
        if (!temp_file.is_open())
        {
            throw WordleSolverException("Could not open eligible words file for writing");
        }

        std::string line;
        while (dataset_file.good())
        {
            std::getline(dataset_file, line);
            temp_file << line << "\n";
        }

        temp_file << std::flush;
        dataset_file.close();
        temp_file.close();
    }
};

std::string WordleSolver::FEEDBACK_PLACEHOLDER = "";
std::string WordleSolver::ELIGIBLE_FP_SUFFIX = "-temp";

WordleSolver::WordleSolver(std::string_view d_fp, BaseRanker *r) : num_guesses(0), ranker(r), dictionary_fp(d_fp), eligible_fp(InsertFilePathSuffix(dictionary_fp, WordleSolver::ELIGIBLE_FP_SUFFIX)) {}

std::string WordleSolver::Guess()
{
    return Guess(FEEDBACK_PLACEHOLDER);
}

std::string WordleSolver::Guess(std::string_view feedback)
{

    if (feedback == FEEDBACK_PLACEHOLDER)
    {
        Private::CopyDataset(*this);
        found_indices.clear();
        num_guesses = 0;
    }
    else
    {
        Private::UpdateEligibleWords(*this, feedback);
    }

    ranker->SetUp(eligible_fp);
    std::string current_guess;
    unsigned int current_min_rank{UINT_MAX};
    unsigned int current_rank;
    std::string word;
    std::ifstream temp_file(eligible_fp, std::ios_base::in);
    if (!temp_file.is_open())
    {
        throw WordleSolverException("Could not open eligible words");
    }
    while (temp_file.good())
    {
        std::getline(temp_file, word);
        current_rank = ranker->Rank(word);
        if (current_rank < current_min_rank)
        {
            current_guess = word;
            current_min_rank = current_rank;
        }
    }
    temp_file.close();
    if (current_guess.empty())
    {
        throw WordleSolverException("Unable to make guess - no eligible words");
    }
    num_guesses++;
    prev_guess = current_guess;
    return current_guess;
}
