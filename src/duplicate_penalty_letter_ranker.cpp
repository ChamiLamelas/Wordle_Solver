// Implementation for duplicate_penalty_letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "duplicate_penalty_letter_ranker.h"
#include <unordered_set>

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