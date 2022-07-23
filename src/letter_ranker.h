/*
Header for LetterRanker.

This file defines the LetterRanker class. This class derives
BaseRanker where the rank a word is derived from the frequency
of its letters in the eligible words.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef LETTER_RANKER_H
#define LETTER_RANKER_H

#include "ranker.h"
#include <unordered_map>

/*
This class defines a ranking scheme based on letter frequency.

Assuming that the wordle solution is selected randomly from the list of
wordle solutions, a guess that is made up of letters that appear frequently
in eligible words is more likely to contain letters in the solution word.
*/
class LetterRanker : public BaseRanker
{
public:
    /*
    Constructs a LetterRanker.
    */
    LetterRanker();

    /*
    Sets up a LetterRanker to rank based on the currently eligible words.

    In particular, each letter is ranked based on its frequency in the eligible
    words (not in the original dictionary).

    Parameters:
        eligible_fp: Path to the remaining eligible words.
    */
    virtual void SetUp(const std::string &eligible_fp) override;

    /*
    Computes the rank of a word as the sum of its letter ranks.

    There is no penalty of duplicate letters.

    Parameters:
        word: Word to rank.

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word) const override;

    /*
    Returns the name of this ranking scheme.
    */
    virtual std::string Name() const override;

    /*
    Gets the count of a letter currently stored by the ranker.

    Parameters:
        letter: A letter

    Returns:
        The count of letter calculated in SetUp or 0 if no count can
        be found for the letter.
    */
    virtual int GetCount(char letter) const;

private:
    /*
    Stores the rank (1,2,...) of each letter that appears in the eligible words.
    Assuming only letters a-z, this could be replaced by a length 26
    int array. This is updated by SetUp which, as described in WordleSolver,
    is called before each guess is made so that ranking can be adjusted
    for the eligible words. Note, the letters that were most frequent
    in the overall dictionary may not the best most frequent after the
    dictionary has been reduced via various rounds of feedback.
    */
    std::unordered_map<char, int> ranking;
};

#endif