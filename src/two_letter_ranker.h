/*
Header for rankers that use two letter substring frequency.

This file defines the TwoLetterRanker, DuplicatePenaltyTwoLetterRanker,
and RestrictedDuplicatePenaltyTwoLetterRanker classes.

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
    std::unordered_map<std::string, size_t> word_counts;

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

/*
This class defines a ranking scheme based on two letter substring frequency, presence of duplicate letters, and guess number.

From TwoLetterRanker:

    Assuming that the wordle solution is selected randomly from the list of
    wordle solutions, a guess that is made up of two letter substrings that
    appear frequently in eligible words is more likely to contain letters
    in the solution word.

However, we may want to keep the letters in the guesses as unique as possible. This class allows
one to put a penalty in the presence of duplicates for the first few guesses, but not after that.

That is, say for the first two guesses:

RestrictedDuplicatePenaltyTwoLetterRanker::Rank(w) = DuplicatePenaltyTwoLetterRanker::Rank(w)

And after that:

RestrictedDuplicatePenaltyTwoLetterRanker::Rank(w) = TwoLetterRanker::Rank(w)

This class provides additional functions as declared in DuplicatePenaltyTwoLetterRanker, only some are overriden.
*/
class RestrictedDuplicatePenaltyTwoLetterRanker final : public DuplicatePenaltyTwoLetterRanker
{
public:
    /*
    Constructs a RestrictedDuplicatePenaltyTwoLetterRanker with a penalty and number of guesses to penalize duplicates.

    Parameters:
        p: Duplicate penalty
        ng: Number of guesses to penalize duplicate.
    */
    RestrictedDuplicatePenaltyTwoLetterRanker(int p, unsigned short ng);

    /*
    Constructs a RestrictedDuplicatePenaltyTwoLetterRanker with a penalty and number of guesses to penalize duplicates.

    Parameters:
        name: Name for the ranker
        p: Duplicate penalty
        ng: Number of guesses to penalize duplicate.
    */
    RestrictedDuplicatePenaltyTwoLetterRanker(std::string_view name, int p, unsigned short ng);

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