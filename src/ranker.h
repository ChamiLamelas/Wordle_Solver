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
        guess: What guess this set up will be for (1...6).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess) = 0;

    /*
    Provides the ranking for a word for a certain guess.

    This function calculates the rank for a word. A rank can be any integer.
    WordleSolver will choose the eligible word w for guess g where Rank(w, g) 
    is the lowest. It is assumed w was seen in the most recent call to SetUp
    which also received g.

    Parameters:
        word: Word to rank.
        guess: What guess this ranking will be for (1...6).

    Returns:
        Rank of word.
    */
    virtual int Rank(std::string_view word, unsigned short guess) const = 0;

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