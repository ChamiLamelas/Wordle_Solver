#include "solver.h"
#include <fstream>
#include <limits>
#include <iostream>
#include "exceptions.h"

std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix)
{
    auto idx = fp.find_last_of('.');
    return fp.substr(0, idx) + suffix + fp.substr(idx);
}

// https://stackoverflow.com/a/28734794
class WordleSolver::Private
{

public:
    template <typename Functor>
    static void KeepOnPredicate(WordleSolver &self, const Functor &predicate)
    {
        std::ifstream temp_file_r(self.temp_fp, std::ios_base::in);
        if (!temp_file_r.is_open())
        {
            throw WordleSolverException("Could not open " + self.temp_fp + " for reading");
        }

        std::string temp2_fp{InsertFilePathSuffix(self.temp_fp, TEMP_FP_SUFFIX)};
        std::fstream temp2_file_rw(temp2_fp, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
        if (!temp2_file_rw.is_open())
        {
            throw WordleSolverException("Could not create or open and clear " + temp2_fp + " for reading or writing");
        }

        bool first_word{true};
        std::string word;
        while (temp_file_r.good())
        {
            std::getline(temp_file_r, word);
            if (!predicate(word))
            {
                continue;
            }

            if (!first_word)
            {
                temp2_file_rw << "\n";
            }

            temp2_file_rw << word;
            if (first_word)
            {
                first_word = false;
            }
        }
        temp_file_r.close();

        std::ofstream temp_file_w(self.temp_fp, std::ios_base::out | std::ios_base::trunc);
        if (!temp_file_w.is_open())
        {
            throw WordleSolverException("Could not open " + self.temp_fp + " for writing");
        }

        temp2_file_rw.clear();
        temp2_file_rw.seekg(0);
        first_word = true;

        while (temp2_file_rw.good())
        {
            std::getline(temp2_file_rw, word);
            if (!first_word)
            {
                temp_file_w << "\n";
            }

            temp_file_w << word;
            if (first_word)
            {
                first_word = false;
            }
        }

        temp2_file_rw.close();
        temp_file_w.close();
    }

    static void RebuildTemp(WordleSolver &self, std::string_view feedback)
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
        std::ifstream dataset_file(self.dataset_fp, std::ios_base::in);
        if (!dataset_file.is_open())
        {
            throw WordleSolverException("Could not open dataset for reading");
        }

        std::ofstream temp_file(self.temp_fp, std::ios_base::out);
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

std::string WordleSolver::PLACEHOLDER = "";
std::string WordleSolver::TEMP_FP_SUFFIX = "-temp";

WordleSolver::WordleSolver(std::string_view d_fp, BaseRanker *r) : num_guesses(0), ranker(r), dataset_fp(d_fp), temp_fp(InsertFilePathSuffix(dataset_fp, WordleSolver::TEMP_FP_SUFFIX)) {}

std::string WordleSolver::Guess()
{
    return Guess(PLACEHOLDER);
}

std::string WordleSolver::Guess(std::string_view feedback)
{
    if (feedback == PLACEHOLDER)
    {
        Private::CopyDataset(*this);
        found_indices.clear();
        num_guesses = 0;
    }
    else
    {
        Private::RebuildTemp(*this, feedback);
    }

    ranker->SetUp(temp_fp);
    std::string current_guess;
    unsigned int current_min_rank{UINT_MAX};
    unsigned int current_rank;
    std::string word;
    std::ifstream temp_file(temp_fp, std::ios_base::in);
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
