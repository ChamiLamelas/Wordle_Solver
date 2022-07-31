// Implementation for letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "letter_ranker.h"
#include <fstream>
#include "misc.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_set>
#include <stdint.h>

LetterRanker::LetterRanker() : AbstractRanker("LetterRanker") {}

LetterRanker::LetterRanker(std::string_view name) : AbstractRanker(name) {}

void LetterRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open " + eligible_fp + " for reading");
    }

    // Stores the current word read from file
    std::string word;

    // Stores the unique characters in word
    std::unordered_set<char> unique_chars;

    // Read over each letter in each eligible word and increase its count after resetting counts.
    word_counts.clear();
    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        for (auto c : word)
        {
            if (unique_chars.find(c) == unique_chars.end())
            {
                word_counts[c]++;
                unique_chars.insert(c);
            }
        }

        // Clear unique characters for next word
        unique_chars.clear();
    }
    eligible_file.close();

    CountsToRanks(word_counts, ranking, letters);
}

int LetterRanker::Rank(std::string_view word, unsigned short guess) const
{
    // Sums rank of letters in word using map
    int rank{0};
    for (const auto &c : word)
    {
        rank += ranking.find(c)->second;
    }
    return rank;
}

std::string LetterRanker::GetDebugInfo() const
{
    // https://stackoverflow.com/questions/18892281/most-optimized-way-of-concatenation-in-strings
    std::string debug_info;
    debug_info.reserve(letters.size() * 10);
    for (auto l : letters)
    {
        debug_info += l;
        debug_info += ": ";
        debug_info += std::to_string(ranking.find(l)->second);
        debug_info += " ";
        debug_info += std::to_string(word_counts.find(l)->second);
        debug_info += "\n";
    }

    return debug_info;
}

SubstringRanker::SubstringRanker(unsigned short n) : SubstringRanker("SubstringRanker(" + std::to_string(n) + ")", n) {}

SubstringRanker::SubstringRanker(std::string_view name, unsigned short n) : AbstractRanker(name)
{
    if (n < 2 || n > 4)
    {
        throw WordleSolverException("Invalid n (must be 2, 3, or 4), use LetterRanker for n = 1");
    }
    substring_len = n;
}

void SubstringRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open " + eligible_fp + " for reading");
    }

    // Stores the current word read from file
    std::string word;

    // Stores the unique n letter substrings in word
    std::unordered_set<std::string> unique_substrs;

    // Read over each substring in each eligible word and increase its count after resetting word_counts.
    word_counts.clear();
    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        for (auto i{0}; i < word.size() - (substring_len - 1); i++)
        {
            auto substr{word.substr(i, substring_len)};
            if (unique_substrs.find(substr) == unique_substrs.end())
            {
                word_counts[substr]++;
                unique_substrs.insert(substr);
            }
        }

        // Clear unique substrings for next word
        unique_substrs.clear();
    }
    eligible_file.close();

    CountsToRanks(word_counts, ranking, substrings);
}

int SubstringRanker::Rank(std::string_view word, unsigned short guess) const
{
    // Sums rank of substrings in word using map
    int rank{0};
    for (auto i{0}; i < word.size() - (substring_len - 1); i++)
    {
        // Can't seem to find with string_views (https://stackoverflow.com/a/53530846)
        // Should find optimization for this - may need to change ranker ADT
        rank += ranking.find(std::string(word.substr(i, substring_len)))->second;
    }
    return rank;
}

std::string SubstringRanker::GetDebugInfo() const
{
    // https://stackoverflow.com/questions/18892281/most-optimized-way-of-concatenation-in-strings
    std::string debug_info;
    debug_info.reserve(substrings.size() * 10);
    for (const auto &s : substrings)
    {
        debug_info += s;
        debug_info += ": ";
        debug_info += std::to_string(ranking.find(s)->second);
        debug_info += " ";
        debug_info += std::to_string(word_counts.find(s)->second);
        debug_info += "\n";
    }

    return debug_info;
}