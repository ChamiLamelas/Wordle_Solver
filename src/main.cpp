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

/*
Main function - by default uses RunUserMode, can alternatively make
use of evaluation header.
*/
int main()
{
    std::string freq_fp{"data/five_letter_freq.csv"};

    RandomRanker r1("RandomRanker(1)", 1);
    LetterRanker r2;
    SubstringRanker r3("2LetterRanker", 2);
    SubstringRanker r4("3LetterRanker", 3);
    DuplicateRanker r5("DuplicateLetterRanker", &r2, 100);
    DuplicateRanker r6("Duplicate2LetterRanker", &r3, 100);
    DuplicateRanker r7("Duplicate3LetterRanker", &r4, 100);
    DuplicateRanker r8("Restricted2DuplicateLetterRanker", &r2, 100, 2);
    DuplicateRanker r9("Restricted2Duplicate2LetterRanker", &r3, 100, 2);
    DuplicateRanker r10("Restricted2Duplicate3LetterRanker", &r4, 100, 2);
    DuplicateRanker r11("Restricted3DuplicateLetterRanker", &r2, 100, 3);
    DuplicateRanker r12("Restricted3Duplicate2LetterRanker", &r3, 100, 3);
    DuplicateRanker r13("Restricted3Duplicate3LetterRanker", &r4, 100, 3);
    PositionRanker r14("PositionRanker");
    DuplicateRanker r15("DuplicatePositionRanker", &r14, 100);
    DuplicateRanker r16("Restricted2DuplicatePositionRanker", &r14, 100, 2);
    DuplicateRanker r17("Restricted3DuplicatePositionRanker", &r14, 100, 3);
    WordFrequencyRanker r18("WordDuplicateLetterRanker", &r5, freq_fp);
    WordFrequencyRanker r19("WordRestricted3DuplicateLetterRanker", &r11, freq_fp);
    WordFrequencyRanker r20("WordDuplicate2LetterRanker", &r6, freq_fp);
    WordFrequencyRanker r21("WordDuplicate3LetterRanker", &r7, freq_fp);
    WordFrequencyRanker r22("WordRestricted3Duplicate2LetterRanker", &r12, freq_fp);
    WordFrequencyRanker r23("WordRestricted3Duplicate3LetterRanker", &r13, freq_fp);
    WordFrequencyRanker r24("WordDuplicatePositionRanker", &r15, freq_fp);
    VowelRanker r25("VowelDuplicateLetterRanker", &r5, 100);
    VowelRanker r26("VowelRestricted3DuplicateLetterRanker", &r11, 100);
    VowelRanker r27("VowelDuplicate2LetterRanker", &r6, 100);
    VowelRanker r28("VowelDuplicate3LetterRanker", &r7, 100);
    VowelRanker r29("VowelRestricted3Duplicate2LetterRanker", &r12, 100);
    VowelRanker r30("VowelRestricted3Duplicate3LetterRanker", &r13, 100);
    VowelRanker r31("VowelDuplicatePositionRanker", &r15, 100, 2);
    VowelRanker r32("Vowel2DuplicateLetterRanker", &r5, 100, 2);
    VowelRanker r33("Vowel2Restricted3DuplicateLetterRanker", &r11, 100, 2);
    VowelRanker r34("Vowel2Duplicate2LetterRanker", &r6, 100, 2);
    VowelRanker r35("Vowel2Duplicate3LetterRanker", &r7, 100, 2);
    VowelRanker r36("Vowel2Restricted3Duplicate2LetterRanker", &r12, 100, 2);
    VowelRanker r37("Vowel2Restricted3Duplicate3LetterRanker", &r13, 100, 2);
    VowelRanker r38("Vowel2DuplicatePositionRanker", &r15, 100, 2);

    std::vector<std::string> dictionary_fps{"data/dracos_github_words.txt", "data/medium_wordle_words_todate.txt"};
    // &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &r13, &r14, &r15, &r16, &r17, &r18, &r19, &r20, &r21, &r22, &r23, &r24, &r25, &r26, &r27, &r28, &r29, &r30, &r31
    // &r32, &r33, &r34, &r35, &r36, &r37, &r38
    std::vector<AbstractRanker *> rankers{&r32, &r33, &r34, &r35, &r36, &r37, &r38};

    try
    {
        // WordleSolver solver(dictionary_fps[1], &r9, true);
        // std::cout << "Evaluation result: " << Evaluate(solver, "lapse") << std::endl;
        RunUserMode(dictionary_fps[0], &r22);
        // GridEvaluate(dictionary_fps, rankers, "data/medium_wordle_words_todate.txt");
    }
    catch (const WordleSolverException &e)
    {
        std::cout << "Guess failure: " << e.what() << std::endl;
    }

    return 0;
}
