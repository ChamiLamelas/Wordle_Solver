// Implementation for combined_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "combined_ranker.h"

CombinedRanker::CombinedRanker() : CombinedRanker(0.25, 0.25, 0.25, 0.25) {}
CombinedRanker::CombinedRanker(double lw, double l2w, double l3w, double pw) : CombinedRanker("CombinedRanker(" + std::to_string(lw) + "," + std::to_string(l2w) + "," + std::to_string(l3w) + "," + std::to_string(pw) + ")") {}
CombinedRanker::CombinedRanker(std::string_view name) : CombinedRanker(name, 0.25, 0.25, 0.25, 0.25) {}
CombinedRanker::CombinedRanker(std::string_view name, double lw, double l2w, double l3w, double pw) : AbstractRanker(name), letter_weight(lw), two_letter_weight(l2w), three_letter_weight(l3w) {}
void CombinedRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    letter_ranker.SetUp(eligible_fp, guess, feedback);
    two_letter_ranker.SetUp(eligible_fp, guess, feedback);
    three_letter_ranker.SetUp(eligible_fp, guess, feedback);
    position_ranker.SetUp(eligible_fp, guess, feedback);
}

int CombinedRanker::Rank(std::string_view word) const
{
    return letter_weight * letter_ranker.Rank(word) + two_letter_weight * two_letter_ranker.Rank(word) + three_letter_weight * three_letter_ranker.Rank(word) + position_weight * position_ranker.Rank(word);
}

std::string CombinedRanker::GetDebugInfo() const
{
    return letter_ranker.GetDebugInfo() + "\n" + two_letter_ranker.GetDebugInfo() + "\n" + three_letter_ranker.GetDebugInfo() + "\n" + position_ranker.GetDebugInfo();
}