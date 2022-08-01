// Implementation for duplicate_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "duplicate_ranker.h"
#include <unordered_set>
#include <algorithm>

DuplicateRanker::DuplicateRanker(AbstractRanker *r, int dp) : DuplicateRanker(r, dp, 7) {}
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

ProgressDuplicateRanker::ProgressDuplicateRanker(AbstractRanker *r, int dp) : ProgressDuplicateRanker(r, dp, 6) {}
ProgressDuplicateRanker::ProgressDuplicateRanker(AbstractRanker *r, int dp, unsigned short nf) : ProgressDuplicateRanker("ProgressDuplicateRanker(" + r->GetName() + "," + std::to_string(dp) + "," + std::to_string(nf) + ")", r, dp, nf) {}
ProgressDuplicateRanker::ProgressDuplicateRanker(std::string_view name, AbstractRanker *r, int dp) : ProgressDuplicateRanker(name, r, dp, 6) {}
ProgressDuplicateRanker::ProgressDuplicateRanker(std::string_view name, AbstractRanker *r, int dp, unsigned short nf) : AbstractRanker(name), ranker(r), duplicate_penalty(dp), num_found(nf) {}

void ProgressDuplicateRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    curr_found = feedback.size() - std::count(feedback.cbegin(), feedback.cend(), 'b');
    ranker->SetUp(eligible_fp, guess, feedback);
}

int ProgressDuplicateRanker::Rank(std::string_view word) const
{
    if (curr_found >= num_found)
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

std::string ProgressDuplicateRanker::GetDebugInfo() const
{
    return ranker->GetDebugInfo();
}
