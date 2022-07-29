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
#include <vector>

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
    Constructs a TwoLetterRanker given a name.

    Parameters:
        name: Name for the ranker
    */
    TwoLetterRanker(std::string_view name);

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
    Gets the rank of a two letter substring currently stored by the ranker.

    Parameters:
        substr: Two letter substring

    Returns:
        The rank of two letter substring calculated in SetUp or INT_MAX if 
        no rank can be found for this substring.
    */
    virtual int GetRank(std::string_view substr) const;

    /*
    Gets a string with substrings ranking and count information.

    Returns:
        A string with each substring stored in the eligible words file most
        recently seen by SetUp with substrings ordered in ascending ranking
        order along with their rank and frequency count on separate lines.
    */
    virtual std::string GetDebugInfo() const override;

private:
    /*
    Stores the frequencies of each two letter substring that appears in 
    the eligible words. If we have words "abbbb" and "aabbb" then 
    word_counts['bb'] = 2 (not 5). That is, it functions akin to performing 
    a grep + wc on the words file. Assuming only letters a-z, this could 
    be replaced by a length (26 c 2) * 2 int array. This is updated by SetUp 
    which, as described in WordleSolver, is called before each guess is made 
    so that the ranking map can be updated. Note, the two letter substrings 
    that were most frequent in the overall dictionary may not the best 
    most frequent after the dictionary has been reduced via various rounds 
    of feedback.
    */
    std::unordered_map<std::string, int> word_counts;

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

    /*
    Stores the keys of ranking and counts, that is the two letter substrings 
    in the eligible words file, sorted by their rank. That is, substrings[0] 
    is the substring that occurs most frequently in the eligible words file.
    */
    std::vector<std::string> substrings;
};

#endif