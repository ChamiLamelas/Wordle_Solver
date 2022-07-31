/*
Header for WordFrequencyRanker.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef WORD_FREQUENCY_RANKER_H
#define WORD_FREQUENCY_RANKER_H

#include "ranker.h"
#include <unordered_map>

/*
A ranker that incorporates how common a word is in a ranking.

This class derives from AbstractRanker and will rank words higher
if they are more popular. This class operates on an AbstractRanker 
that will actually rank the word (e.g. LetterRanker or SubstringRanker). 
To better understand the relationship of WordFrequencyRanker and the
AbstractRanker it uses, see the comments of the member functions
below.
*/
class WordFrequencyRanker : public AbstractRanker
{
public:
    /*
    Constructs a WordFrequencyRanker.

    Parameters:
        name: Name for the ranker
        r: Pointer to AbstractRanker that will be used by this class.
        freq_fp: Filepath to CSV file with 2 columns where the first column
                 is a list of 5 letter words ordered in descending popularity
                 and the second column is the counts. Popularity ranks are
                 computed from this file. Words with equal frequency have
                 equal rank.
    */
    WordFrequencyRanker(std::string_view name, AbstractRanker *r, const std::string &freq_fp);

    /*
    Sets up this ranker by calling SetUp on the AbstractRanker passed via the constructor.

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess);

    /*
    Computes a rank using the passed AbstractRanker and the popularity ranking.

    The returned rank is computed as the sum of the rank computed by the passed
    ranker and the popularity rank of the word. If we have no popularity information
    on the word, then it is assigned a rank R that is guaranteed to be higher than
    all other calculated ranks. All words with no popularity information from
    the passed frequency file will be assigned the same value R.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word, unsigned short guess) const;

    /*
    Gets debug information on this ranker and the passed ranker.

    Returns:
        A string starting with the popularity ranking information
        calculated by this ranker followed by the debug information
        provided by GetDebugInfo() in the passed ranker.
    */
    virtual std::string GetDebugInfo() const;
private:
    // Ranking of words by popularity constructed using freq_fp
    std::unordered_map<std::string, int> ranking;

    // Passed ranker (used to implement SetUp, Rank, GetDebugInfo)
    AbstractRanker *ranker;
};

#endif