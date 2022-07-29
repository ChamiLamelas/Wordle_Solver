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
    RandomRanker rr("RandomRanker(1)", 1);
    LetterRanker lr; // Default name ok
    DuplicatePenaltyLetterRanker dplr("DuplicatePenaltyLetterRanker(100)", 100);
    TwoLetterRanker tlr; // Default name ok
    DuplicatePenaltyTwoLetterRanker dptlr("DuplicatePenaltyTwoLetterRanker(100)", 100);
    RestrictedDuplicatePenaltyLetterRanker rdplr2("RestrictedDuplicatePenaltyLetterRanker(100,2)", 100, 2);
    RestrictedDuplicatePenaltyTwoLetterRanker rdptlr2("RestrictedDuplicatePenaltyTwoLetterRanker(100,2)", 100, 2);
    RestrictedDuplicatePenaltyLetterRanker rdplr3("RestrictedDuplicatePenaltyLetterRanker(100,3)", 100, 3);
    RestrictedDuplicatePenaltyTwoLetterRanker rdptlr3("RestrictedDuplicatePenaltyTwoLetterRanker(100,3)", 100, 3);

    std::vector<std::string> dictionary_fps{"data/dracos_github_words.txt"};
    // , &dplr, &tlr, &dptlr, &rdplr2, &rdptlr2, &rdplr3, &rdptlr3
    std::vector<AbstractRanker *> rankers{&rdplr2};

    try
    {
        WordleSolver solver(dictionary_fps[0], &rdplr2, true);
        std::cout << "Evaluation result: " << Evaluate(solver, "bench") << std::endl;
        // RunUserMode(dictionary_fps[0], &lr, true);
        // GridEvaluate(dictionary_fps, rankers, "data/medium_wordle_words_todate.txt");
    }
    catch (const WordleSolverException &e)
    {
        std::cout << "Guess failure: " << e.what() << std::endl;
    }

    return 0;
}
