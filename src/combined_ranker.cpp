// Implementation for combined_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "combined_ranker.h"

CombinedRanker::CombinedRanker() : CombinedRanker("CombinedRanker()") {}
CombinedRanker::CombinedRanker(std::string_view name) : AbstractRanker(name) {}
void CombinedRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    letter_ranker.SetUp(eligible_fp, guess, feedback);
    two_letter_ranker.SetUp(eligible_fp, guess, feedback);
    three_letter_ranker.SetUp(eligible_fp, guess, feedback);
    position_ranker.SetUp(eligible_fp, guess, feedback);
}

int CombinedRanker::Rank(std::string_view word) const
{
    return letter_ranker.Rank(word) + two_letter_ranker.Rank(word) + three_letter_ranker.Rank(word) + position_ranker.Rank(word);
}

std::string CombinedRanker::GetDebugInfo() const
{
    return letter_ranker.GetDebugInfo() + "\n" + two_letter_ranker.GetDebugInfo() + "\n" + three_letter_ranker.GetDebugInfo() + "\n" + position_ranker.GetDebugInfo();
}