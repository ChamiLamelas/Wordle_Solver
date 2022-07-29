/*
Header for rankers that use single letter frequency.

This file defines the LetterRanker, DuplicatePenaltyLetterRanker,
and RestrictedDuplicatePenaltyLetterRanker classes.

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
class DuplicatePenaltyLetterRanker : public LetterRanker
{
public:
    /*
    Constructs a DuplicatePenaltyLetterRanker with a penalty.

    Parameters:
        p: Duplicate penalty
    */
    DuplicatePenaltyLetterRanker(int p);

    /*
    Constructs a DuplicatePenaltyLetterRanker with a penalty.

    Parameters:
        name: Name for the ranker
        p: Duplicate penalty
    */
    DuplicatePenaltyLetterRanker(std::string_view name, int p);

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
    Constructs a RestrictedDuplicatePenaltyLetterRanker with a penalty and number of guesses to penalize duplicates.

    Parameters:
        name: Name for the ranker
        p: Duplicate penalty
        ng: Number of guesses to penalize duplicate.
    */
    RestrictedDuplicatePenaltyLetterRanker(std::string_view name, int p, unsigned short ng);

    /*
    Computes the rank of a word as described in the class comment.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        The rank as described above.
    */
    int Rank(std::string_view word, unsigned short guess) const override;

private:
    // The number of guesses for which duplicates will be penalized
    unsigned short num_guesses;
};

#endif