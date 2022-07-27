#ifndef RESTRICTED_DUPLICATE_PENALTY_TWO_LETTER_RANKER_H
#define RESTRICTED_DUPLICATE_PENALTY_TWO_LETTER_RANKER_H

#include "duplicate_penalty_two_letter_ranker.h"

class RestrictedDuplicatePenaltyTwoLetterRanker final : public DuplicatePenaltyTwoLetterRanker
{
public:
    RestrictedDuplicatePenaltyTwoLetterRanker(int p, unsigned short ng);
    int Rank(std::string_view word, unsigned short guess) const override;
    std::string Name() const override;

private:
    unsigned short num_guesses;
};

#endif