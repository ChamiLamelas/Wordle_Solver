/*
Header for PositionRanker

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef POSITION_RANKER_H
#define POSITION_RANKER_H

#include "ranker.h"
#include <unordered_map>
#include <vector>

/*
Class that ranks words based on the letters at each position in the word.

PositionRanker, as opposed to LetterRanker and SubstringRanker, ranks
words based on the letters that appear at particular positions in the
word. LetterRanker and SubstringRanker rank based on letters or
substrings appear anywhere in the word. Ranks derived by PositionRanker
come from frequency statistics computed on the eligible words of letters
at each of the 5 positions in the word.
*/
class PositionRanker : public AbstractRanker
{
public:
    /*
    Constructs a PositionRanker.
    */
    PositionRanker();

    /*
    Constructs a PositionRanker with a name.

    Parameters:
        name: Name for the ranker
    */
    PositionRanker(std::string_view name);

    /*
    Sets up a PositionRanker to rank based on the currently eligible words.

    In particular, each letter is ranked based on its frequency at each of
    the 5 positions in the eligible words (not in the original dictionary).

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
        feedback: Feedback on previous guess (if guess = 1, this parameter should not
                  be used by SetUp, it will be set to some placeholder when called).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback) override;

    /*
    Computes the rank of a word as the sum of the ranks of each letter at their indices.

    There is no penalty of duplicate letters.

    Parameters:
        word: Word to rank.

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word) const override;

    /*
    Gets a string with letters ranking and count information for each of 5 indices.

    Returns:
        A string with each letter stored in the eligible words file most
        recently seen by SetUp with letters ordered in ascending ranking
        order along with their rank and frequency count on separate lines
        for each of the 5 positions.
    */
    virtual std::string GetDebugInfo() const override;

private:
    // word_counts[i] is a map of each letter to the number of
    // words it occurs in at position i in the eligible words
    std::unordered_map<char, size_t> word_counts[5];

    // rankings[i] is a map of each letter to its rank for
    // occurring at position i in the eligible words (derived
    // from word_counts[i] in SetUp)
    std::unordered_map<char, int> rankings[5];

    // letters[i] is a vector of the letters at position i
    // stored according to ascending rank (derived from
    // word_counts[i] in SetUp)
    std::vector<char> letters[5];
};

#endif