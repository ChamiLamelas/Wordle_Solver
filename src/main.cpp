// Client code (including main())
// Author: Chami Lamelas
// Date: Summer 2022

#include <iostream>
#include "user.h"
#include "evaluation.h"
#include "exceptions.h"
#include "ranker.h"
#include "random_ranker.h"
#include "solver.h"
#include "letter_ranker.h"
#include "duplicate_penalty_letter_ranker.h"
#include "two_letter_ranker.h"
#include "duplicate_penalty_two_letter_ranker.h"
#include "restricted_duplicate_penalty_letter_ranker.h"
#include "restricted_duplicate_penalty_two_letter_ranker.h"

/*
Main function - by default uses RunUserMode, can alternatively make
use of evaluation header.
*/
int main()
{
    RandomRanker rr(1);
    LetterRanker lr;
    DuplicatePenaltyLetterRanker dplr(100);
    TwoLetterRanker tlr;
    DuplicatePenaltyTwoLetterRanker dptlr(100);
    RestrictedDuplicatePenaltyLetterRanker rdplr(100, 2);
    RestrictedDuplicatePenaltyTwoLetterRanker rdptlr(100, 2);

    std::vector<std::string> dictionary_fps{"data/dracos_github_words.txt"};
    std::vector<BaseRanker *> rankers{&rr};

    try
    {
        // RunUserMode(dictionary_fps[0], rankers[0]);
        GridEvaluate(dictionary_fps, rankers, "data/medium_wordle_words_todate.txt");
    }
    catch (const WordleSolverException &e)
    {
        std::cout << "Guess failure: " << e.what() << std::endl;
    }

    return 0;
}
