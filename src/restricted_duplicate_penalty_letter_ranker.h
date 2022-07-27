/*
Header for RestrictedDuplicatePenaltyLetterRanker.

This file defines the RestrictedDuplicatePenaltyLetterRanker class. 
This class derives DuplicatePenaltyLetterRanker where the rank a word is 
derived from the frequency of its letters in the eligible 
words,the presence of duplicate letters, and the number of
the guess being made.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef RESTRICTED_DUPLICATE_PENALTY_LETTER_RANKER_H
#define RESTRICTED_DUPLICATE_PENALTY_LETTER_RANKER_H

#include "duplicate_penalty_letter_ranker.h"

/*
This class defines a ranking scheme based on letter frequency, presence of duplicate letters, and guess number.

From LetterRanker:

    Assuming that the wordle solution is selected randomly from the list of
    wordle solutions, a guess that is made up of letters that appear frequently
    in eligible words is more likely to contain letters in the solution word.

However, we may want to keep the letters in the guesses as unique as possible. This class allows
one to put a penalty in the presence of duplicates for the first few guesses, but not after that. 

That is, say for the first two guesses:

RestrictedDuplicatePenaltyLetterRanker::Rank(w) = DuplicatePenaltyLetterRanker::Rank(w)

And after that:

RestrictedDuplicatePenaltyLetterRanker::Rank(w) = LetterRanker::Rank(w)

This class provides additional functions as declared in DuplicatePenaltyLetterRanker, only some are overriden.
*/
class RestrictedDuplicatePenaltyLetterRanker final : public DuplicatePenaltyLetterRanker
{
public:
    /*
    Constructs a RestrictedDuplicatePenaltyLetterRanker with a penalty and number of guesses to penalize duplicates.

    Parameters:
        p: Duplicate penalty
        ng: Number of guesses to penalize duplicate.
    */
    RestrictedDuplicatePenaltyLetterRanker(int p, unsigned short ng);

    /*
    Computes the rank of a word as described in the class comment.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        The rank as described above.
    */
    int Rank(std::string_view word, unsigned short guess) const override;

    /*
    Returns the name of this ranking scheme.
    */
    std::string Name() const override;

private:
    // The number of guesses for which duplicates will be penalized
    unsigned short num_guesses;
};

#endif