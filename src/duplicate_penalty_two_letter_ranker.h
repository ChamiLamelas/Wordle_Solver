#ifndef DUPLICATE_PENALTY_TWO_LETTER_RANKER_H
#define DUPLICATE_PENALTY_TWO_LETTER_RANKER_H

#include "two_letter_ranker.h"

class DuplicatePenaltyTwoLetterRanker : public TwoLetterRanker
{
public:
    DuplicatePenaltyTwoLetterRanker(int p);
    virtual int Rank(std::string_view word, unsigned short guess) const override;
    virtual std::string Name() const override;

private:
    int duplicate_penalty;
};

#endif