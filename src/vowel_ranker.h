/*
Header for VowelRanker.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef VOWEL_RANKER_H
#define VOWEL_RANKER_H

#include "ranker.h"
#include <unordered_set>

// Set of vowels used by VowelRanker (initialized here because too
// complicated to initialize static const members that are objects)
const std::unordered_set<char> VOWELS{'a', 'e', 'i', 'o', 'u'};

/*
A ranker that penalizes the presences of consonants in another ranker.

This class derives from AbstractRanker and will penalize the presence
of consonants when ranking a word. A user can control for how many
guesses the penalty is applied. For instance, after a few guesses,
one may want to be more lenient on the solver choosing consonants
in words. This class operates on an AbstractRanker that will
actually rank the word (e.g. LetterRanker or SubstringRanker). To
better understand the relationship of VowelRanker and the
AbstractRanker it uses, see the comments of the member functions
below. The idea behind this class is that it may be good to use
vowels at the beginning over consontants as more words could
potentially be eliminated for future guesses.
*/
class VowelRanker : public AbstractRanker
{
public:
    /*
    Constructs a VowelRanker where consonants are always penalized.

    Parameters:
        r: Pointer to AbstractRanker that will be used by this class.
        cp: Penalty applied for each consontant letter.
    */
    VowelRanker(AbstractRanker *r, int cp);

    /*
    Overloaded version of VowelRanker(AbstractRanker*,int) that limits when consonants are penalized.

    Parameters:
        r: Pointer to AbstractRanker that will be used by this class.
        cp: Penalty applied for each consontant letter.
        ng: Number of guesses for which the penalty is applied.
    */
    VowelRanker(AbstractRanker *r, int cp, unsigned short ng);

    /*
    Overloaded version of VowelRanker(AbstractRanker*,int) that allows name specification.

    Parameters:
        name: Name for the ranker
        r: Pointer to AbstractRanker that will be used by this class.
        cp: Penalty applied for each consonant.
    */
    VowelRanker(std::string_view name, AbstractRanker *r, int cp);

    /*
    Overloaded version of VowelRanker(AbstractRanker*,int,unsigned short) that allows name specification.

    Parameters:
        name: Name for the ranker
        r: Pointer to AbstractRanker that will be used by this class.
        cp: Penalty applied for each consonant.
        ng: Number of guesses for which the penalty is applied.
    */
    VowelRanker(std::string_view name, AbstractRanker *r, int cp, unsigned short ng);

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
    Computes a rank using the passed AbstractRanker and potentially a consonant penalty.

    If we have not yet surpassed the number of guesses for which consonants are being
    penalized, the returned rank is computed as the sum of the passed ranker's Rank()
    output and the passed consonant penalty for each instance of a consonant. If we
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

    // Consonant penalty (cp in constructors) applied to each consontant
    int consonant_penalty;

    // Number of guesses for which consontants will be penalized (ng in constructors)
    unsigned short num_guesses;

    // Stores current guess reported by SetUp
    unsigned short curr_guess;
};

#endif