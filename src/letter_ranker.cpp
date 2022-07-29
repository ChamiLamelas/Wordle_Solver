// Implementation for letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "letter_ranker.h"
#include <fstream>
#include "exceptions.h"
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
            if (unique_chars.find(c) == unique_chars.end()) {
                word_counts[c] += 1;
                unique_chars.insert(c);
            }
        }

        // Clear unique characters for next word
        unique_chars.clear();
    }
    eligible_file.close();

    letters.clear();
    for (const auto &p : word_counts)
    {
        letters.push_back(p.first);
    }

    // Sort the letters based on their frequency
    // word_counts isn't in enclosing scope, this->word_counts is truly what is
    // called everytime word_counts is written, so we pass this to the
    // capture list (which is a pointer)
    // Note letters must be a vector as sort requires random
    // access iterators: (https://cplusplus.com/reference/algorithm/sort/)
    std::sort(letters.begin(), letters.end(), [this](char k1, char k2)
              { return word_counts[k1] > word_counts[k2]; });

    // Update our ranking map (clear removes any letters that may no longer be in eligible words)
    ranking.clear();
    int curr_rank{0};
    size_t curr_count{SIZE_MAX};
    for (auto l : letters) {
        if (word_counts[l] < curr_count) {
            curr_rank++;
            curr_count = word_counts[l];
        }
        ranking[l] = curr_rank;
    }
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

int LetterRanker::GetRank(char letter) const
{
    auto itr{ranking.find(letter)};
    return (itr == ranking.end()) ? INT_MAX : itr->second;
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

DuplicatePenaltyLetterRanker::DuplicatePenaltyLetterRanker(int p) : LetterRanker("DuplicatePenaltyLetterRanker"), duplicate_penalty(p) {}
DuplicatePenaltyLetterRanker::DuplicatePenaltyLetterRanker(std::string_view name, int p) : LetterRanker(name), duplicate_penalty(p) {}

int DuplicatePenaltyLetterRanker::Rank(std::string_view word, unsigned short guess) const
{
    // Sums rank of letters in word using GetCount, identifies duplicates
    std::unordered_set<char> uniq_letters;
    int rank{0};
    for (const auto &c : word)
    {
        rank += GetRank(c);
        uniq_letters.insert(c);
    }
    return rank + (duplicate_penalty * (word.size() - uniq_letters.size()));
}

RestrictedDuplicatePenaltyLetterRanker::RestrictedDuplicatePenaltyLetterRanker(int p, unsigned short ng): DuplicatePenaltyLetterRanker("RestrictedDuplicatePenaltyLetterRanker", p), num_guesses(ng) {}

RestrictedDuplicatePenaltyLetterRanker::RestrictedDuplicatePenaltyLetterRanker(std::string_view name, int p, unsigned short ng): DuplicatePenaltyLetterRanker(name, p), num_guesses(ng) {}

int RestrictedDuplicatePenaltyLetterRanker::Rank(std::string_view word, unsigned short guess) const {
    return (guess <= num_guesses) ? DuplicatePenaltyLetterRanker::Rank(word, guess) : LetterRanker::Rank(word, guess);
}