/*
Header for RandomRanker.

This file defines the RandomRanker class. This class is a simple
derived class of AbstractRanker. This ranking scheme results in the
WordleSolver selecting eligible words randomly when guessing.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef RANDOM_RANKER_H
#define RANDOM_RANKER_H

#include "ranker.h"

/*
Class that ranks eligible words randomly.

This class derives AbstractRanker and prvoides random ranks for words. This
class provides a simple implementation of AbstractRanker and also a useful
baseline for comparing more advanced ranking schemes. This class cannot
be derived from.
*/
class RandomRanker final : public AbstractRanker
{
public:
    /*
    Constructs a RandomRanker with non reproducible rankings.

    The current time is used in ranking eligible words, which means that
    results of using a WordleSolver with this ranking scheme will not
    be reproducible even for the same solution word. An overloaded
    version of this constructor that takes a name is not provided as
    an non reproducible ranker is assumed to not be used for evaluation
    and this class is final anyway.
    */
    RandomRanker();

    /*
    Constructs a RandomRanker with reproducible rankings.

    Since a seed is used in ranking eligible words, the results of using
    a WordleSolver with this ranking scheme will be reproducible for the
    same solution word. In fact, using the same dictionary, the same
    initial guess will always be made.

    Parameters:
        seed: Specifies random seed to be used in ranking eligible words.
    */
    RandomRanker(unsigned int seed);

    /*
    Constructs a RandomRanker with reproducible rankings.

    Since a seed is used in ranking eligible words, the results of using
    a WordleSolver with this ranking scheme will be reproducible for the
    same solution word. In fact, using the same dictionary, the same
    initial guess will always be made.

    Parameters:
        name: Name for the ranker
        seed: Specifies random seed to be used in ranking eligible words.
    */
    RandomRanker(std::string_view name, unsigned int seed);

    /*
    Sets up the RandomRanker for ranking.

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
    */
    void SetUp(const std::string &eligible_fp, unsigned short guess) override;

    /*
    Provides a random rank for a word.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        Random rank between 0 and RAND_MAX see
        https://cplusplus.com/reference/cstdlib/RAND_MAX/.
    */
    int Rank(std::string_view word, unsigned short guess) const override;
};

#endif