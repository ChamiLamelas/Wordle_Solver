/*
Header for RandomRanker.

This file defines the RandomRanker class. This class is a simple
derived class of BaseRanker. This ranking scheme results in the
WordleSolver selecting eligible words randomly when guessing.
*/

#ifndef RANDOM_RANKER_H
#define RANDOM_RANKER_H

#include "ranker.h"

/*
Class that ranks eligible words randomly.

This class derives BaseRanker and prvoides random ranks for words. This
class provides a simple implementation of BaseRanker and also a useful
baseline for comparing more advanced ranking schemes. This class cannot
be derived from.
*/
class RandomRanker final : public BaseRanker
{
public:
    /*
    Constructs a RandomRanker with non reproducible rankings.

    The current time is used in ranking eligible words, which means that
    results of using a WordleSolver with this ranking scheme will not
    be reproducible even for the same solution word.
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
    Sets up the RandomRanker for ranking.

    Parameters:
        eligible_fp: Path to the remaining eligible words.
    */
    void SetUp(const std::string &eligible_fp) override;

    /*
    Provides a random rank for a word.

    Parameters:
        word: Word to rank.

    Returns:
        Random rank between 0 and RAND_MAX see
        https://cplusplus.com/reference/cstdlib/RAND_MAX/.
    */
    int Rank(std::string_view word) override;

    /*
    Returns the name of this ranking scheme.
    */
    std::string Name() const override;
};

#endif