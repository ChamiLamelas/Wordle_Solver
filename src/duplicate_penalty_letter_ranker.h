/*
Header for DuplicatePenaltyLetterRanker.

This file defines the DuplicatePenaltyLetterRanker class. 
This class derives LetterRanker where the rank a word is 
derived from the frequency of its letters in the eligible 
words as well as from the presence of duplicate letters.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef DUPLICATE_PENALTY_LETTER_RANKER_H
#define DUPLICATE_PENALTY_LETTER_RANKER_H

#include "letter_ranker.h"

/*
This class defines a ranking scheme based on letter frequency and presence of duplicate letters.

From LetterRanker:

    Assuming that the wordle solution is selected randomly from the list of
    wordle solutions, a guess that is made up of letters that appear frequently
    in eligible words is more likely to contain letters in the solution word.

However, we may want to keep the letters in the guesses as unique as possible. This class allows
one to put a penalty in the presence of duplicates. That is, 

DuplicatePenaltyLetterRanker::Rank(w) = LetterRanker::Rank(w) + penalty * (# duplicate letters in w)

This class provides additional functions as declared in LetterRanker, only some are overriden.
*/
class DuplicatePenaltyLetterRanker final : public LetterRanker
{
public:
    /*
    Constructs a DuplicatePenaltyLetterRanker with a penalty.

    Parameters:
        p: Duplicate penalty
    */
    DuplicatePenaltyLetterRanker(int p);

    /*
    Computes the rank of a word as described in the class comment.

    Parameters:
        word: Word to rank.

    Returns:
        The rank as described above.
    */
    int Rank(std::string_view word) const override;

    /*
    Returns the name of this ranking scheme.
    */
    std::string Name() const override;

private:
    // Duplicate penalty as described above
    int duplicate_penalty;
};

#endif