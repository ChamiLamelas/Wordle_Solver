/*
Header for DuplicateRanker.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef DUPLICATE_RANKER_H
#define DUPLICATE_RANKER_H

#include "ranker.h"

/*
A ranker that penalizes the presences of duplicates in another ranker.

This class derives from AbstractRanker and will penalize the presence
of duplicates when ranking a word. A user can control for how many
guesses the penalty is applied. For instance, after a few guesses,
one may want to be more lenient on the solver choosing duplicate
letter words. This class operates on an AbstractRanker that will
actually rank the word (e.g. LetterRanker or SubstringRanker). To
better understand the relationship of DuplicateRanker and the
AbstractRanker it uses, see the comments of the member functions
below.
*/
class DuplicateRanker : public AbstractRanker
{
public:
    /*
    Constructs a DuplicateRanker where duplicates are always penalized.

    Parameters:
        r: Pointer to AbstractRanker that will be used by this class.
        dp: Penalty applied for each duplicate letter.
    */
    DuplicateRanker(AbstractRanker *r, int dp);

    /*
    Overloaded version of DuplicateRanker(AbstractRanker*,int) that limits when duplicates are penalized.

    Parameters:
        r: Pointer to AbstractRanker that will be used by this class.
        dp: Penalty applied for each duplicate letter.
        ng: Number of guesses for which the penalty is applied.
    */
    DuplicateRanker(AbstractRanker *r, int dp, unsigned short ng);

    /*
    Overloaded version of DuplicateRanker(AbstractRanker*,int) that allows name specification.

    Parameters:
        name: Name for the ranker
        r: Pointer to AbstractRanker that will be used by this class.
        dp: Penalty applied for each duplicate letter.
    */
    DuplicateRanker(std::string_view name, AbstractRanker *r, int dp);

    /*
    Overloaded version of DuplicateRanker(AbstractRanker*,int,unsigned short) that allows name specification.

    Parameters:
        name: Name for the ranker
        r: Pointer to AbstractRanker that will be used by this class.
        dp: Penalty applied for each duplicate letter.
        ng: Number of guesses for which the penalty is applied.
    */
    DuplicateRanker(std::string_view name, AbstractRanker *r, int dp, unsigned short ng);

    /*
    Sets up this ranker by calling SetUp on the AbstractRanker passed via the constructor.

    Parameters:
        eligible_fp: Path to the remaining eligible words.
        guess: What guess this set up will be for (1...6).
        feedback: Feedback on previous guess (if guess = 1, this parameter should not
                  be used by SetUp, it will be set to some placeholder when called).
    */
    virtual void SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback) override;

    /*
    Computes a rank using the passed AbstractRanker and potentially a duplicate penalty.

    If we have not yet surpassed the number of guesses for which duplicates are being
    penalized, the returned rank is computed as the sum of the passed ranker's Rank()
    output and the passed duplicate penalty for each instance of a duplicate. If we
    have surpassed it, then we do not apply the penalty, and only return the passed
    ranker's Rank().

    Parameters:
        word: Word to rank.

    Returns:
        The rank as computed above.
    */
    virtual int Rank(std::string_view word) const override;

    // Returns GetDebugInfo() as defined by the passed ranker.
    virtual std::string GetDebugInfo() const override;

private:
    // Passed ranker (used to implement SetUp, Rank, GetDebugInfo)
    AbstractRanker *ranker;

    // Duplicate penalty (dp in constructors) applied to each duplicate instance
    int duplicate_penalty;

    // Number of guesses for which duplicates will be penalized (ng in constructors)
    unsigned short num_guesses;

    // Stores current guess reported by SetUp
    unsigned short curr_guess;
};

#endif