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
#include "two_letter_ranker.h"
#include "three_letter_ranker.h"

/*
Main function - by default uses RunUserMode, can alternatively make
use of evaluation header.
*/
int main()
{
    RandomRanker r1("RandomRanker(1)", 1);
    LetterRanker r2;    
    TwoLetterRanker r3; 
    ThreeLetterRanker r4;
    DuplicatePenaltyLetterRanker r5("DuplicatePenaltyLetterRanker(100)", 100);
    DuplicatePenaltyTwoLetterRanker r6("DuplicatePenaltyTwoLetterRanker(100)", 100);
    DuplicatePenaltyThreeLetterRanker r7("DuplicatePenaltyThreeLetterRanker(100)", 100);
    RestrictedDuplicatePenaltyLetterRanker r8("RestrictedDuplicatePenaltyLetterRanker(100,2)", 100, 2);
    RestrictedDuplicatePenaltyTwoLetterRanker r9("RestrictedDuplicatePenaltyTwoLetterRanker(100,2)", 100, 2);
    RestrictedDuplicatePenaltyThreeLetterRanker r10("RestrictedDuplicatePenaltyThreeLetterRanker(100,2)", 100, 2);
    RestrictedDuplicatePenaltyLetterRanker r11("RestrictedDuplicatePenaltyLetterRanker(100,3)", 100, 3);
    RestrictedDuplicatePenaltyTwoLetterRanker r12("RestrictedDuplicatePenaltyTwoLetterRanker(100,3)", 100, 3);
    RestrictedDuplicatePenaltyThreeLetterRanker r13("RestrictedDuplicatePenaltyThreeLetterRanker(100,3)", 100, 3);

    // 
    std::vector<std::string> dictionary_fps{"data/dracos_github_words.txt"};
    // &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &r13
    std::vector<AbstractRanker *> rankers{&r4};

    try
    {
        WordleSolver solver(dictionary_fps[0], &r4, true);  
        std::cout << "Evaluation result: " << Evaluate(solver, "cigar") << std::endl;
        // RunUserMode(dictionary_fps[0], &lr, true);
        // GridEvaluate(dictionary_fps, rankers, "data/medium_wordle_words_todate.txt");
    }
    catch (const WordleSolverException &e)
    {
        std::cout << "Guess failure: " << e.what() << std::endl;
    }

    return 0;
}
