/*
Header for AbstractRanker.

This file declares the AbstractRanker abstract class. It specifies the necessary
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

The derived classes of AbstractRanker will implement particular ranking schemes.
*/
class AbstractRanker
{
public:
    /*
    AbstractRanker destructor, not user called.

    Default destructor, declared virtual to ensure proper destruction of objects of
    derived classes.
    */
    virtual ~AbstractRanker();

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
    virtual std::string Name() const;

protected:
    /*
    Sets the name of the ranker. 
    
    Derived classes should provide at least two types of constructors. The first
    type of constructor should not take a name only the parameters necessary to
    construct the derived object. This is useful when using the solver in user
    mode where a name is not important. The second type of constructor should not
    take a name in addition to the parameters necessary to construct the derived
    object. This is useful for two reasons. The first is so that derived objects
    can be constructed with detailed names (possibly involving parameters) for
    better evaluation results. The second reason is so that classes that have
    AbstractRanker as grandparent can also pass a name all the way to the
    AbstractRanker via their constructors which take names.
    */
    AbstractRanker(std::string_view n);

private:
    // Name of the ranker
    std::string name;
};

#endif