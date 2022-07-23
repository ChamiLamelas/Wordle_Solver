#include "duplicate_penalty_two_letter_ranker.h"
#include <unordered_set>

DuplicatePenaltyTwoLetterRanker::DuplicatePenaltyTwoLetterRanker(int p) : duplicate_penalty(p) {}

int DuplicatePenaltyTwoLetterRanker::Rank(std::string_view word) const
{
    std::unordered_set<char> uniq_letters{word[0]};
    // Sums rank of substrings in word using map
    int rank{0};
    for (auto i{0}; i < word.size() - 1; i++)
    {
        rank += GetRank(word.substr(i, 2));
        uniq_letters.insert(word[i + 1]);
    }
    return rank + (duplicate_penalty * (word.size() - uniq_letters.size()));
}

std::string DuplicatePenaltyTwoLetterRanker::Name() const
{
    return "DuplicatePenaltyTwoLetterRanker";
}