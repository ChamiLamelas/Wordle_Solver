/*
Header for LetterRanker.

This file defines the LetterRanker class. This class derives
AbstractRanker where the rank a word is derived from the frequency
of its letters in the eligible words.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef LETTER_RANKER_H
#define LETTER_RANKER_H

#include "ranker.h"
#include <unordered_map>
#include <vector>

/*
This class defines a ranking scheme based on letter frequency.

Assuming that the wordle solution is selected randomly from the list of
wordle solutions, a guess that is made up of letters that appear frequently
in eligible words is more likely to contain letters in the solution word.
*/
class LetterRanker : public AbstractRanker
{
public:
    /*
    Constructs a LetterRanker.
    */
    LetterRanker();

    /*
    Constructs a LetterRanker with a name.

    Parameters:
        name: Name for the ranker
    */
    LetterRanker(std::string_view name);

    /*
    Sets up a LetterRanker to rank based on the currently eligible words.

    In particular, each letter is ranked based on its frequency in the eligible
    words (not in the original dictionary).

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess) override;

    /*
    Computes the rank of a word as the sum of its letter ranks.

    There is no penalty of duplicate letters.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word, unsigned short guess) const override;

    /*
    Gets the rank of a letter currently stored by the ranker.

    Parameters:
        letter: A letter

    Returns:
        The rank of letter calculated in SetUp or INT_MAX if no rank can be
        found for this letter.
    */
    virtual int GetRank(char letter) const;

    /*
    Gets a string with letters ranking and count information.

    Returns:
        A string with each letter stored in the eligible words file most
        recently seen by SetUp with letters ordered in ascending ranking
        order along with their rank and frequency count on separate lines.
    */
    virtual std::string GetDebugInfo() const override;

private:
    /*
    Stores the frequencies of each letter that appears in the eligible words.
    If we have words "abbbb" and "aabbb" then word_counts['a'] = 2 (not 3).
    That is, it functions akin to performing a grep + wc on the words file.
    Assuming only letters a-z, this could be replaced by a length 26 int
    array. This is updated by SetUp which, as described in WordleSolver,
    is called before each guess is made so that the ranking map can be
    updated. Note, the letters that were most frequent in the overall
    dictionary may not the best most frequent after the dictionary has
    been reduced via various rounds of feedback.
    */
    std::unordered_map<char, int> word_counts;

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

    /*
    Stores the keys of ranking and counts, that is the letters in the eligible
    words file, sorted by their rank. That is, letters[0] is the letter that
    occurs most frequently in the eligible words file.
    */
    std::vector<char> letters;
};

#endif