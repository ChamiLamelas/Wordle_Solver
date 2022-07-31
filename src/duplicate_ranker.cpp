// Implementation for duplicate_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "duplicate_ranker.h"
#include <unordered_set>

DuplicateRanker::DuplicateRanker(AbstractRanker *r, int dp) : DuplicateRanker("DuplicateRanker(" + r->GetName() + "," + std::to_string(dp) + ")", r, dp, 7) {}
DuplicateRanker::DuplicateRanker(AbstractRanker *r, int dp, unsigned short ng) : DuplicateRanker("DuplicateRanker(" + r->GetName() + "," + std::to_string(dp) + "," + std::to_string(ng) + ")", r, dp, ng) {}
DuplicateRanker::DuplicateRanker(std::string_view name, AbstractRanker *r, int dp) : DuplicateRanker(name, r, dp, 7) {}
DuplicateRanker::DuplicateRanker(std::string_view name, AbstractRanker *r, int dp, unsigned short ng) : AbstractRanker(name), ranker(r), duplicate_penalty(dp), num_guesses(ng) {}

void DuplicateRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    curr_guess = guess;
    ranker->SetUp(eligible_fp, guess, feedback);
}

int DuplicateRanker::Rank(std::string_view word) const
{
    if (curr_guess > num_guesses)
    {
        return ranker->Rank(word);
    }

    std::unordered_set<char> uniq_letters;
    for (auto c : word)
    {
        uniq_letters.insert(c);
    }
    return ranker->Rank(word) + (duplicate_penalty * (word.size() - uniq_letters.size()));
}

std::string DuplicateRanker::GetDebugInfo() const
{
    return ranker->GetDebugInfo();
}
