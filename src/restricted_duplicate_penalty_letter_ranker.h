#ifndef RESTRICTED_DUPLICATE_PENALTY_LETTER_RANKER_H
#define RESTRICTED_DUPLICATE_PENALTY_LETTER_RANKER_H

#include "duplicate_penalty_letter_ranker.h"

class RestrictedDuplicatePenaltyLetterRanker final : public DuplicatePenaltyLetterRanker
{
public:
    RestrictedDuplicatePenaltyLetterRanker(int p, unsigned short ng);
    int Rank(std::string_view word, unsigned short guess) const override;
    std::string Name() const override;
private:
    unsigned short num_guesses;
};

#endif