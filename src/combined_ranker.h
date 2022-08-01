/*
Header for CombinedRanker.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef COMBINED_RANKER_H
#define COMBINED_RANKER_H

#include "ranker.h"
#include "letter_ranker.h"
#include "position_ranker.h"

/*
This class defines a ranking scheme based on a weighted average of others.

This ranker works using a LetterRanker, a SubstringRanker with n = 2, a
SubstringRanker with n = 3, and a PositionRanker.
*/
class CombinedRanker : public AbstractRanker
{
public:
    /*
    Constructs a CombinedRanker.

    Each used ranker will have an equal weighting.
    */
    CombinedRanker();

    /*
    Overloaded constructor that allows one to assign weights to the 4 used rankers.

    Parameters:
        lw: Weight of LetterRanker
        l2w: Weight of SubstringRanker with n=2
        l3w: Weight of SubstringRanker with n=3
        pw: Weight of PositionRanker
    */
    CombinedRanker(double lw, double l2w, double l3w, double pw);

    /*
    Overloaded version of CombinedRanker() that allows name specification.

    Parameters:
        name: Name for the ranker
    */
    CombinedRanker(std::string_view name);

    /*
    Overloaded version of CombinedRanker(double,double,double,double) that allows name specification.

    Parameters:
        name: Name for the ranker
        lw: Weight of LetterRanker
        l2w: Weight of SubstringRanker with n=2
        l3w: Weight of SubstringRanker with n=3
        pw: Weight of PositionRanker
    */
    CombinedRanker(std::string_view name, double lw, double l2w, double l3w, double pw);

    /*
    Sets up a CombinedRanker to rank based on the currently eligible words.

    Runs SetUp for each of 4 used rankers.

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
        feedback: Feedback on previous guess (if guess = 1, this parameter should not
                  be used by SetUp, it will be set to some placeholder when called).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback) override;

    /*
    Computes the rank of a word as the weighted sum of the Rank outputs for the 4 used rankers.

    There is no penalty of duplicate letters.

    Parameters:
        word: Word to rank.

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word) const override;

    // Returns a string of debug info returned by each of used rankers separated by blank lines
    virtual std::string GetDebugInfo() const override;

private:
    // The 4 rankers, note cannot call constructors with () because C++ thinks
    // this a function declaration is happening (see: https://stackoverflow.com/a/21101550)
    // Using {} initialization instead - don't know what you're supposed to do
    // if object has constructor that takes initialization list
    LetterRanker letter_ranker;
    SubstringRanker two_letter_ranker{2};
    SubstringRanker three_letter_ranker{3};
    PositionRanker position_ranker;

    // Weights for above rankers
    double letter_weight;
    double two_letter_weight;
    double three_letter_weight;
    double position_weight;
};

#endif