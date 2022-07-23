#ifndef DUPLICATE_PENALTY_TWO_LETTER_RANKER_H
#define DUPLICATE_PENALTY_TWO_LETTER_RANKER_H

#include "two_letter_ranker.h"

class DuplicatePenaltyTwoLetterRanker final : public TwoLetterRanker
{
public:
    DuplicatePenaltyTwoLetterRanker(int p);
    int Rank(std::string_view word) const override;
    std::string Name() const override;

private:
    int duplicate_penalty;
};

#endif