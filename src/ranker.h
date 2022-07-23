/*
Header for BaseRanker.

This file declares the BaseRanker abstract class. It specifies the necessary
methods for its derived classes which will implement specific word ranking
strategies that are used by WordleSolver.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef RANKER_H
#define RANKER_H

#include <string_view>
#include <string>

/*
Abstract class that defines the necessary behavior of a ranking scheme.

The derived classes of BaseRanker will implement particular ranking schemes.
*/
class BaseRanker
{
public:
    /*
    BaseRanker destructor, not user called.

    Default destructor, declared virtual to ensure proper destruction of objects of
    derived classes.
    */
    virtual ~BaseRanker(){};

    /*
    Sets up the ranking scheme.

    This pure virtual function declaration is provided to enable derived classes to do
    any necessary set up based on the eligible words to rank properly (with the
    next function). This could be used to calculate necessary statistics, set
    up containers used by Rank, etc. See LetterRanker::SetUp for an example. 

    Parameters:
        eligible_fp: Path to the remaining eligible words.
    */
    virtual void SetUp(const std::string &eligible_fp) = 0;

    /*
    Provides the ranking for a word.

    This function calculates the rank for a word. A rank can be any integer.
    WordleSolver will choose the eligible word w where Rank(w) is the lowest.

    Parameters:
        word: Word to rank.

    Returns:
        Rank of word.
    */
    virtual int Rank(std::string_view word) = 0;

    /*
    Provides a name for the ranker.

    This function provides a name for the ranker, this is used when comparing
    how well the WordleSolver does with different ranking schemes by the
    evaluation header.

    Returns:
        Name for this ranking scheme.
    */
    virtual std::string Name() const = 0;
};

#endif