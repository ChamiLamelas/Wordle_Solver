/*
Header for DuplicatePenaltyTwoLetterRanker.

This file defines the DuplicatePenaltyTwoLetterRanker class. 
This class derives TwoLetterRanker where the rank a word is 
derived from the frequency of its letters in the eligible 
words as well as from the presence of duplicate letters.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef DUPLICATE_PENALTY_TWO_LETTER_RANKER_H
#define DUPLICATE_PENALTY_TWO_LETTER_RANKER_H

#include "two_letter_ranker.h"

/*
This class defines a ranking scheme based on two letter frequency and presence of duplicate letters.

From TwoLetterRanker:

    Assuming that the wordle solution is selected randomly from the list of
    wordle solutions, a guess that is made up of two letter substrings that 
    appear frequently in eligible words is more likely to contain letters 
    in the solution word.

However, we may want to keep the letters in the guesses as unique as possible. This class allows
one to put a penalty in the presence of duplicates. That is, 

DuplicatePenaltyTwoLetterRanker::Rank(w) = TwoLetterRanker::Rank(w) + penalty * (# duplicate letters in w)

This class provides additional functions as declared in TwoLetterRanker, only some are overriden.
*/
class DuplicatePenaltyTwoLetterRanker : public TwoLetterRanker
{
public:
    /*
    Constructs a DuplicatePenaltyTwoLetterRanker with a penalty.

    Parameters:
        p: Duplicate penalty
    */
    DuplicatePenaltyTwoLetterRanker(int p);

    /*
    Constructs a DuplicatePenaltyTwoLetterRanker with a penalty.

    Parameters:
        name: Name for the ranker
        p: Duplicate penalty
    */
    DuplicatePenaltyTwoLetterRanker(std::string_view name, int p);

    /*
    Computes the rank of a word as described in the class comment.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        The rank as described above.
    */
    virtual int Rank(std::string_view word, unsigned short guess) const override;

private:
    // Duplicate penalty as described above
    int duplicate_penalty;
};

#endif