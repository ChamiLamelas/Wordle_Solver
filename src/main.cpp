// Client code (including main())
// Author: Chami Lamelas
// Date: Summer 2022

#include <iostream>
#include "user.h"
#include "evaluation.h"
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
    VowelRanker r31(&r15, 100);
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
    ProgressDuplicateRanker r46(&r2, 100);
    ProgressDuplicateRanker r47(&r3, 100);
    ProgressDuplicateRanker r48(&r4, 100);
    ProgressDuplicateRanker r49(&r2, 100, 3);
    ProgressDuplicateRanker r50(&r3, 100, 3);
    ProgressDuplicateRanker r51(&r4, 100, 3);
    ProgressDuplicateRanker r52(&r2, 100, 4);
    ProgressDuplicateRanker r53(&r3, 100, 4);
    ProgressDuplicateRanker r54(&r4, 100, 4);
    ProgressDuplicateRanker r55(&r14, 100);
    ProgressDuplicateRanker r56(&r14, 100, 3);
    ProgressDuplicateRanker r57(&r14, 100, 4);
    ProgressVowelRanker r58(&r5, 100);
    ProgressVowelRanker r59(&r11, 100);
    ProgressVowelRanker r60(&r6, 100);
    ProgressVowelRanker r61(&r7, 100);
    ProgressVowelRanker r62(&r12, 100);
    ProgressVowelRanker r63(&r13, 100);
    ProgressVowelRanker r64(&r15, 100);
    ProgressVowelRanker r65(&r5, 100, 2);
    ProgressVowelRanker r66(&r11, 100, 2);
    ProgressVowelRanker r67(&r6, 100, 2);
    ProgressVowelRanker r68(&r7, 100, 2);
    ProgressVowelRanker r69(&r12, 100, 2);
    ProgressVowelRanker r70(&r13, 100, 2);
    ProgressVowelRanker r71(&r15, 100, 2);
    ProgressVowelRanker r72(&r41, 100);
    ProgressVowelRanker r73(&r41, 100, 2);
    ProgressWordFrequencyRanker r74(&r5, freq_fp);
    ProgressWordFrequencyRanker r75(&r11, freq_fp);
    ProgressWordFrequencyRanker r76(&r6, freq_fp);
    ProgressWordFrequencyRanker r77(&r7, freq_fp);
    ProgressWordFrequencyRanker r78(&r12, freq_fp);
    ProgressWordFrequencyRanker r79(&r13, freq_fp);
    ProgressWordFrequencyRanker r80(&r15, freq_fp);
    ProgressWordFrequencyRanker r81(&r41, freq_fp);
    ProgressWordFrequencyRanker r82(&r5, freq_fp, 3);
    ProgressWordFrequencyRanker r83(&r11, freq_fp, 3);
    ProgressWordFrequencyRanker r84(&r6, freq_fp, 3);
    ProgressWordFrequencyRanker r85(&r7, freq_fp, 3);
    ProgressWordFrequencyRanker r86(&r12, freq_fp, 3);
    ProgressWordFrequencyRanker r87(&r13, freq_fp, 3);
    ProgressWordFrequencyRanker r88(&r15, freq_fp, 3);
    ProgressWordFrequencyRanker r89(&r41, freq_fp, 3);
    ProgressWordFrequencyRanker r90(&r5, freq_fp, 4);
    ProgressWordFrequencyRanker r91(&r11, freq_fp, 4);
    ProgressWordFrequencyRanker r92(&r6, freq_fp, 4);
    ProgressWordFrequencyRanker r93(&r7, freq_fp, 4);
    ProgressWordFrequencyRanker r94(&r12, freq_fp, 4);
    ProgressWordFrequencyRanker r95(&r13, freq_fp, 4);
    ProgressWordFrequencyRanker r96(&r15, freq_fp, 4);
    ProgressWordFrequencyRanker r97(&r41, freq_fp, 4);
    ProgressDuplicateRanker r98(&r39, 100);
    ProgressDuplicateRanker r99(&r39, 100, 3);
    ProgressDuplicateRanker r100(&r39, 100, 4);

    CombinedRanker rl1;
    ProgressDuplicateRanker rl2(&rl1, 100, 4);
    ProgressVowelRanker rl3(&rl2, 2);
    ProgressWordFrequencyRanker rl4(&rl3, freq_fp, 3);

    std::vector<std::string> dictionary_fps{"data/dracos_github_words.txt", "data/medium_wordle_words_todate.txt"};
    // &r1,&r2,&r3,&r4,&r5,&r6,&r7,&r8,&r9,&r10,&r11,&r12,&r13,&r14,&r15,&r16,&r17,&r18,&r19,&r20,&r21,&r22,&r23,&r24,&r25,&r26,&r27,&r28,&r29,&r30,&r31,&r32,&r33,&r34,&r35,&r36,&r37,&r38,&r39,&r40,&r41,&r42,&r43,&r44,&r45,&r46,&r47,&r48,&r49,&r50,&r51,&r52,&r53,&r54,&r55,&r56,&r57,&r58,&r59,&r60,&r61,&r62,&r63,&r64,&r65,&r66,&r67,&r68,&r69,&r70,&r71,&r72,&r73,&r74,&r75,&r76,&r77,&r78,&r79,&r80,&r81,&r82,&r83,&r84,&r85,&r86,&r87,&r88,&r89,&r90,&r91,&r92,&r93,&r94,&r95,&r96,&r97,&r98,&r99,&r100
    std::vector<AbstractRanker *> rankers{&rl4};

    if (argc == 2 && strcmp(argv[1], "-e") == 0)
    {
        GridEvaluate(dictionary_fps, rankers, "data/medium_wordle_words_todate.txt");
    }
    else if (argc == 2 && strcmp(argv[1], "-d") == 0)
    {
        WordleSolver solver(dictionary_fps[0], &r22, true);
        std::cout << "Evaluation result: " << Evaluate(solver, "parry") << std::endl;
        // std::cout << GetFeedback("erect", "runes") << " " << GetFeedback("trees", "runes") << std::endl; 
    }
    else
    {
        RunUserMode(dictionary_fps[0], &rl4);
    }

    return 0;
}
