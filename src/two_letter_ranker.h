/*
Header for TwoLetterRanker.

This file defines the TwoLetterRanker class. This class derives
AbstractRanker where the rank a word is derived from the frequency
of its two letter substrings in the eligible words.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef TWO_LETTER_RANKER_H
#define TWO_LETTER_RANKER_H

#include "ranker.h"
#include <unordered_map>

/*
This class defines a ranking scheme based on two letter substring frequency.

Assuming that the wordle solution is selected randomly from the list of
wordle solutions, a guess that is made up of two letter substrings that 
appear frequently in eligible words is more likely to contain letters 
in the solution word.
*/
class TwoLetterRanker : public AbstractRanker
{
public:
    /*
    Constructs a TwoLetterRanker.
    */
    TwoLetterRanker();

    /*
    Sets up a TwoLetterRanker to rank based on the currently eligible words.

    In particular, each two letter substring is ranked based on its frequency 
    in the eligible words (not in the original dictionary).

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess) override;

    /*
    Computes the rank of a word as the sum of its two letter substring ranks.

    There is no penalty of duplicate letters.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word, unsigned short guess) const override;

    /*
    Returns the name of this ranking scheme.
    */
    virtual std::string Name() const override;

    /*
    Gets the rank of a two letter substring currently stored by the ranker.

    Parameters:
        substr: Two letter substring

    Returns:
        The rank of two letter substring calculated in SetUp or INT_MAX if 
        no rank can be found for this substring.
    */
    virtual int GetRank(std::string_view substr) const;

    /*
    Gets the count of a two letter substring currently stored by the ranker.

    Parameters:
        substr: Two letter substring

    Returns:
        The count of two letter substring calculated in SetUp or 0 if 
        no count can be found for this substring.
    */
    virtual int GetCount(std::string_view substr) const;

private:
    /*
    Stores the frequencies of each two letter substring that appears in 
    the eligible words. Assuming only letters a-z, this could be replaced 
    by a length (26 c 2) * 2 int array. This is updated by SetUp which, 
    as described in WordleSolver, is called before each guess is made so 
    that the ranking map can be updated. Note, the two letter substrings 
    that were most frequent in the overall dictionary may not the best 
    most frequent after the dictionary has been reduced via various rounds 
    of feedback.
    */
    std::unordered_map<std::string, int> counts;

    /*
    Stores the rank (1,2,...) of each two letter substring that appears 
    in the eligible words. Assuming only letters a-z, this could be 
    replaced by a length (26 c 2) * 2 int array. This is updated by 
    SetUp which, as described in WordleSolver, is called before each 
    guess is made so that ranking can be adjusted for the eligible words. 
    Note, the two letter substrings that were most frequent in the 
    overall dictionary may not the best most frequent after the
    dictionary has been reduced via various rounds of feedback.
    */
    std::unordered_map<std::string, int> ranking;
};

#endif