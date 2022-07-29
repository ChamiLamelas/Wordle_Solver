// Implementation for three_letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "three_letter_ranker.h"
#include <fstream>
#include "exceptions.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_set>

ThreeLetterRanker::ThreeLetterRanker() : AbstractRanker("ThreeLetterRanker") {}

ThreeLetterRanker::ThreeLetterRanker(std::string_view name) : AbstractRanker(name) {}

void ThreeLetterRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open " + eligible_fp + " for reading");
    }

    // Read over each substring in each eligible word and increase its count after resetting counts.
    std::string word;
    std::unordered_set<std::string> unique_substrs;
    word_counts.clear();
    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        for (auto i{0}; i < word.size() - 2; i++)
        {
            auto substr{word.substr(i, 3)};
            if (unique_substrs.find(substr) == unique_substrs.end())
            {
                word_counts[substr]++;
                unique_substrs.insert(substr);
            }
        }
        unique_substrs.clear();
    }
    eligible_file.close();

    substrings.clear();
    for (const auto &p : word_counts)
    {
        substrings.push_back(p.first);
    }

    // Sort the letters based on their frequency
    std::sort(substrings.begin(), substrings.end(), [this](const std::string &k1, const std::string &k2)
              { return word_counts[k1] > word_counts[k2]; });

    // Update our ranking map (clear removes any substrings that may no longer be in eligible words)
    ranking.clear();
    for (size_t i{0}; i < substrings.size(); i++)
    {
        ranking[substrings[i]] = i + 1;
    }
}

int ThreeLetterRanker::Rank(std::string_view word, unsigned short guess) const
{
    // Sums rank of substrings in word using map
    int rank{0};
    for (auto i{0}; i < word.size() - 2; i++)
    {
        // Can't seem to find with string_views (https://stackoverflow.com/a/53530846)
        // Should find optimization for this - may need to change ranker ADT
        rank += ranking.find(std::string(word.substr(i, 3)))->second;
    }
    return rank;
}

int ThreeLetterRanker::GetRank(std::string_view substr) const
{
    auto itr{ranking.find(std::string(substr))};
    return (itr == ranking.end()) ? INT_MAX : itr->second;
}

std::string ThreeLetterRanker::GetDebugInfo() const
{
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

DuplicatePenaltyThreeLetterRanker::DuplicatePenaltyThreeLetterRanker(int p) : ThreeLetterRanker("DuplicatePenaltyThreeLetterRanker"), duplicate_penalty(p) {}
DuplicatePenaltyThreeLetterRanker::DuplicatePenaltyThreeLetterRanker(std::string_view name, int p) : ThreeLetterRanker(name), duplicate_penalty(p) {}

int DuplicatePenaltyThreeLetterRanker::Rank(std::string_view word, unsigned short guess) const
{
    std::unordered_set<char> uniq_letters{word[0], word[1]};
    // Sums rank of substrings in word using map
    int rank{0};
    for (auto i{0}; i < word.size() - 2; i++)
    {
        rank += GetRank(word.substr(i, 3));
        uniq_letters.insert(word[i + 2]);
    }
    return rank + (duplicate_penalty * (word.size() - uniq_letters.size()));
}

RestrictedDuplicatePenaltyThreeLetterRanker::RestrictedDuplicatePenaltyThreeLetterRanker(int p, unsigned short ng) : DuplicatePenaltyThreeLetterRanker("RestrictedDuplicatePenaltyTwoLetterRanker", p), num_guesses(ng) {}

RestrictedDuplicatePenaltyThreeLetterRanker::RestrictedDuplicatePenaltyThreeLetterRanker(std::string_view name, int p, unsigned short ng) : DuplicatePenaltyThreeLetterRanker(name, p), num_guesses(ng) {}

int RestrictedDuplicatePenaltyThreeLetterRanker::Rank(std::string_view word, unsigned short guess) const
{
    return (guess <= num_guesses) ? DuplicatePenaltyThreeLetterRanker::Rank(word, guess) : ThreeLetterRanker::Rank(word, guess);
}