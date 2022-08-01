// Client code (including main())
// Author: Chami Lamelas
// Date: Summer 2022

#include <iostream>
#include "user.h"
#include "evaluation.h"
#include "misc.h"
#include "ranker.h"
#include "random_ranker.h"
#include "solver.h"
#include "letter_ranker.h"
#include "duplicate_ranker.h"
#include "position_ranker.h"
#include "word_frequency_ranker.h"
#include "vowel_ranker.h"
#include <cstring>
#include "combined_ranker.h"

/*
Main function - by default uses RunUserMode, can alternatively make
use of evaluation header.
*/
int main(int argc, char *argv[])
{
    std::string freq_fp{"data/five_letter_freq.csv"};

    RandomRanker r1(1);
    LetterRanker r2;
    SubstringRanker r3(2);
    SubstringRanker r4(3);
    DuplicateRanker r5(&r2, 100);
    DuplicateRanker r6(&r3, 100);
    DuplicateRanker r7(&r4, 100);
    DuplicateRanker r8(&r2, 100, 2);
    DuplicateRanker r9(&r3, 100, 2);
    DuplicateRanker r10(&r4, 100, 2);
    DuplicateRanker r11(&r2, 100, 3);
    DuplicateRanker r12(&r3, 100, 3);
    DuplicateRanker r13(&r4, 100, 3);
    PositionRanker r14;
    DuplicateRanker r15(&r14, 100);
    DuplicateRanker r16(&r14, 100, 2);
    DuplicateRanker r17(&r14, 100, 3);
    WordFrequencyRanker r18(&r5, freq_fp);
    WordFrequencyRanker r19(&r11, freq_fp);
    WordFrequencyRanker r20(&r6, freq_fp);
    WordFrequencyRanker r21(&r7, freq_fp);
    WordFrequencyRanker r22(&r12, freq_fp);
    WordFrequencyRanker r23(&r13, freq_fp);
    WordFrequencyRanker r24(&r15, freq_fp);
    VowelRanker r25(&r5, 100);
    VowelRanker r26(&r11, 100);
    VowelRanker r27(&r6, 100);
    VowelRanker r28(&r7, 100);
    VowelRanker r29(&r12, 100);
    VowelRanker r30(&r13, 100);
    VowelRanker r31(&r15, 100, 2);
    VowelRanker r32(&r5, 100, 2);
    VowelRanker r33(&r11, 100, 2);
    VowelRanker r34(&r6, 100, 2);
    VowelRanker r35(&r7, 100, 2);
    VowelRanker r36(&r12, 100, 2);
    VowelRanker r37(&r13, 100, 2);
    VowelRanker r38(&r15, 100, 2);
    CombinedRanker r39;
    DuplicateRanker r40(&r39, 100);
    DuplicateRanker r41(&r39, 100, 2);
    DuplicateRanker r42(&r39, 100, 3);
    VowelRanker r43(&r41, 100);
    VowelRanker r44(&r41, 100, 2);
    WordFrequencyRanker r45(&r41, freq_fp);

    std::vector<std::string> dictionary_fps{"data/dracos_github_words.txt", "data/medium_wordle_words_todate.txt"};
    // &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &r13, &r14, &r15, &r16, &r17, &r18, &r19, &r20, &r21, &r22, &r23, &r24, &r25, &r26, &r27, &r28, &r29, &r30, &r31
    // &r32, &r33, &r34, &r35, &r36, &r37, &r38, &r39, &r40, &r41, &r42, &r43, &r44, &r45
    std::vector<AbstractRanker *> rankers{&r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &r13, &r14, &r15, &r16, &r17, &r18, &r19, &r20, &r21, &r22, &r23, &r24, &r25, &r26, &r27, &r28, &r29, &r30, &r31, &r32, &r33, &r34, &r35, &r36, &r37, &r38, &r39, &r40, &r41, &r42, &r43, &r44, &r45};

    try
    {
        if (argc == 2 && strcmp(argv[1], "-e") == 0)
        {
            GridEvaluate(dictionary_fps, rankers, "data/medium_wordle_words_todate.txt");
        }
        else if (argc == 2 && strcmp(argv[1], "-d") == 0)
        {
            WordleSolver solver(dictionary_fps[0], &r22, true);
            std::cout << "Evaluation result: " << Evaluate(solver, "parry") << std::endl;
        }
        else
        {
            RunUserMode(dictionary_fps[0], &r22);
        }
    }
    catch (const WordleSolverException &e)
    {
        std::cout << "Guess failure: " << e.what() << std::endl;
    }

    return 0;
}
