#include "duplicate_penalty_letter_ranker.h"
#include <unordered_set>

DuplicatePenaltyLetterRanker::DuplicatePenaltyLetterRanker(int p) : duplicate_penalty(p) {}

int DuplicatePenaltyLetterRanker::Rank(std::string_view word) const
{
    // Sums rank of letters in word using GetCount, identifies duplicates
    std::unordered_set<char> uniq_letters;
    int rank{0};
    for (const auto &c : word)
    {
        rank += GetCount(c);
        uniq_letters.insert(c);
    }
    return rank + (duplicate_penalty * (word.size() - uniq_letters.size()));
}

std::string DuplicatePenaltyLetterRanker::Name() const
{
    return "DuplicatePenaltyLetterRanker";
}