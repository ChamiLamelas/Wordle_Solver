// Implementation for two_letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "two_letter_ranker.h"
#include <fstream>
#include "exceptions.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

TwoLetterRanker::TwoLetterRanker() {}

void TwoLetterRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open " + eligible_fp + " for reading");
    }

    // Read over each substring in each eligible word and increase its count after resetting counts.
    std::string word;
    counts.clear();
    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        for (auto i{0}; i < word.size() - 1; i++)
        {
            counts[word.substr(i, 2)]++;
        }
    }
    eligible_file.close();

    // Collect the letters in the eligible words into a vector, container
    // to be sorted must have random access iterators
    // (https://cplusplus.com/reference/algorithm/sort/)
    std::vector<std::string> keys;
    for (const auto &p : counts)
    {
        keys.push_back(p.first);
    }

    // Sort the letters based on their frequency
    std::sort(keys.begin(), keys.end(), [this](const std::string &k1, const std::string &k2)
              { return counts[k1] > counts[k2]; });

    // Update our ranking map (clear removes any substrings that may no longer be in eligible words)
    ranking.clear();
    for (size_t i{0}; i < keys.size(); i++)
    {
        ranking[keys[i]] = i + 1;
    }
}

int TwoLetterRanker::Rank(std::string_view word, unsigned short guess) const
{
    // Sums rank of substrings in word using map
    int rank{0};
    for (auto i{0}; i < word.size() - 1; i++)
    {
        // Can't seem to find with string_views (https://stackoverflow.com/a/53530846)
        // Should find optimization for this - may need to change ranker ADT
        rank += ranking.find(std::string(word.substr(i, 2)))->second;
    }
    return rank;
}

std::string TwoLetterRanker::Name() const
{
    return "TwoLetterRanker";
}

int TwoLetterRanker::GetRank(std::string_view substr) const
{
    auto itr{ranking.find(std::string(substr))};
    return (itr == ranking.end()) ? INT_MAX : itr->second;
}

int TwoLetterRanker::GetCount(std::string_view substr) const
{
    auto itr{counts.find(std::string(substr))};
    return (itr == counts.end()) ? 0 : itr->second;
}