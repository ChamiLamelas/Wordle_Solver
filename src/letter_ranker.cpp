// Implementation for letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "letter_ranker.h"
#include <fstream>
#include "exceptions.h"
#include <vector>
#include <algorithm>
#include <limits>

LetterRanker::LetterRanker(){};

void LetterRanker::SetUp(const std::string &eligible_fp)
{
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open " + eligible_fp + " for reading");
    }

    // Read over each letter in each eligible word and increase its count after resetting counts.
    counts.clear();
    std::string word;
    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        for (const auto &c : word)
        {
            counts[c] += 1;
        }
    }
    eligible_file.close();

    // Collect the letters in the eligible words into a vector, container
    // to be sorted must have random access iterators
    // (https://cplusplus.com/reference/algorithm/sort/)
    std::vector<char> keys;
    for (const auto &p : counts)
    {
        keys.push_back(p.first);
    }

    // Sort the letters based on their frequency
    // counts isn't in enclosing scope, this->counts is truly what is
    // called everytime counts is written, so we pass this to the
    // capture list (which is a pointer)
    std::sort(keys.begin(), keys.end(), [this](char k1, char k2)
              { return counts[k1] > counts[k2]; });

    // Update our ranking map (clear removes any letters that may no longer be in eligible words)
    ranking.clear();
    for (size_t i{0}; i < keys.size(); i++)
    {
        ranking[keys[i]] = i + 1;
    }
}

int LetterRanker::Rank(std::string_view word) const
{
    // Sums rank of letters in word using map
    int rank{0};
    for (const auto &c : word)
    {
        rank += ranking.find(c)->second;
    }
    return rank;
}

std::string LetterRanker::Name() const
{
    return "LetterRanker";
}

int LetterRanker::GetRank(char letter) const
{
    auto itr{ranking.find(letter)};
    return (itr == ranking.end()) ? INT_MAX : itr->second;
}

int LetterRanker::GetCount(char letter) const
{
    auto itr{counts.find(letter)};
    return (itr == counts.end()) ? 0 : itr->second;
}