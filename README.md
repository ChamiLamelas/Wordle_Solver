# Wordle Solver

This is a personal project for reviewing C++. The project examines different ways of solving the [Wordle](https://www.nytimes.com/games/wordle/index.html) game. The project has two components. The first is the WordleSolver which tracks which words are eligible based on the feedback from previous guesses. The second component is the ranking scheme. The WordleSolver uses the ranking scheme to pick the next guess from the eligible words. This implementation allows for the easy construction of a variety of ranking schemes.

*Summer 2022*

## Getting Started

These instructions will allow you to replicate building and running an executable using Visual Studio Code on Windows 10 as I did during development.

### Prerequisites

You will need the following software.
- [Visual Studio Code](https://code.visualstudio.com/)
- [MinGW-w64](https://winlibs.com/) - I installed the Win64 Zip archive under the latest release [here](https://winlibs.com/#download-release). As of July 2022, the latest is the one with GCC 12.1.0.

### Installing

Follow the instructions in the following three sections [here](https://github.com/ChamiLamelas/Cpp_Review/blob/main/README.md):
- Installation of C++ Compilers (Windows 10)
- Setup of Visual Studio Code
- Building and Running C++ Program

## Running

Once this has been done, pressing Ctrl+Shift+B builds `wordle_solver.exe` which can be run. Running `wordle_solver.exe` will allow a user to solve the online Wordle game with the solver (see [here](src/user.h) for more information). To set up evaluation experiments and do debugging, you can use `-e` and `-d` command line arguments respectively.

## Results

The ranking schemes I started with were:
* Words are ranked based on the letters they contain (see [LetterRanker](src/letter_ranker.h)).
* Words are ranked based on 2 and 3 letter substrings they contain (see [SubstringRanker](src/letter_ranker.h)).
* Words are ranked based on the letters at their positions (see [PositionRanker](src/position_ranker.h)).
* A combination of the previous 3 (see [CombinedRanker](src/combined_ranker.h)).

I evaluated them using the first 300 Wordle words (see evaluation details [here](src/evaluation.h)). Of these schemes, the last 2 performed the best on their own. For more information, see the starting_rankers sheet [here](data/workbook.xlsx). We then built 3 new ranking schemes that made use of these. These modified rank based on the presence of duplicates, consonants, and the word popularity. The effect of these modifications is discussed below, more details can be found in the sheets of [workbook.xlsx](data/workbook.xlsx). One good ranker based on these results can be found in the selected_rankers sheet [here](data/workbook.xlsx).

### Effects of Penalizing Duplicates

For ranking based on the frequency of contained letters and of the letters at their positions, restricting duplicates based on the number of guesses versus the number of letters marked yellow or green has no difference. However, waiting later into guessing (i.e. more guesses or more correct letters) does yield better results. Restricting duplicates in any manner yields better performance. For ranking based on the frequency of 2 and 3 letter substrings, restricting duplicates based on progress versus number of guesses doesn't impact performance. Furthermore, there is no clear trend in waiting till later to no longer restrict duplicates in guesses. However, restricting duplicates in someway does improve performance. The effect of restricting duplicates for the combined ranking scheme is akin to the effect on ranking using 2 and 3 letter substring frequencies. To see the specific implementation, see [DuplicateRanker and ProgressDuplicateRanker](src/duplicate_ranker.h).

### Effects of Vowel Priority

For all 4 ranking schemes described above, incorporating a penalty on consonants improves performance. Incorporating a penalty based on progress toward the solution instead of based on the number of guesses does better. Vowels should not be penalized for too long. To see the specific implementation, see [VowelRanker and ProgressVowelRanker](src/vowel_ranker.h).

### Effects of Word Popularity

For ranking based on the frequency of contained letters, the effect of word popularity should be incorporated after getting around 4 characters right but doing so earlier also performs well. For the other four ranking schemes, the effect of word popularity being incorporated throughout the guessing process (despite having such a large weighting on the computed ranks) performs the best. For all 4 ranking schemes, incorporating word popularity in anyway yields an improvement. To see the specific implementation, see [WordFrequencyRanker and ProgressWordFrequencyRanker](src/word_frequency_ranker.h).

The dictionary also has a large impact, as seen when using the evaluation set as the dictionary, the performance of the rankers greatly improve. To see all experimental results, see [medium_wordle_words_todate-output_all.txt](data/medium_wordle_words_todate-output_all.txt). 

## Further Development

One can easily add additional ranking schemes provided with the `AbstractRanker` abstract data type defined [here](src/ranker.h) and test them either via user mode or via the evaluation header in `main.cpp`. One could also add additional evaluation metrics. The user mode could also be greatly improved as it was not the focus to allow for specification of different ranking schemes or dictionaries via command line arguments. 

## Additional Information

Please see [the UML diagram](uml_diagram.png) and the documentation in header and implementation files.

## Built With

  - [Visual Studio Code](https://code.visualstudio.com/)
  - [MinGW-w64](https://winlibs.com/) - Specifically GCC 12.0.1 (C++ 20).

## Authors

  - **Chami Lamelas** - *Developer* - [website](https://sites.google.com/brandeis.edu/chamilamelas)

## License

This project is licensed under the [MIT License](LICENSE) - see the [LICENSE](LICENSE) file for
details.

## Acknowledgments

  - I tried to incorporate as many of the concepts covered in my [C++ Review](https://github.com/ChamiLamelas/Cpp_Review).
  - I got the Wordle words till July 2022 from [this Medium article](https://medium.com/@owenyin/here-lies-wordle-2021-2027-full-answer-list-52017ee99e86)
  - I initially used [this word list](https://github.com/tabatkins/wordle-list) from [tabatkins](https://github.com/tabatkins) as my wordle dictionary, but found it was missing at least three words (see [this issue](https://github.com/tabatkins/wordle-list/issues/10)). I later used [this word list](https://gist.github.com/dracos/dd0668f281e685bad51479e5acaadb93) from [dracos](https://gist.github.com/dracos).
  - I used [this README template](https://github.com/PurpleBooth/a-good-readme-template#readme) from [PurpleBooth](https://github.com/PurpleBooth).
  - I got five letter frequencies from [this Kaggle dataset](https://www.kaggle.com/datasets/rtatman/english-word-frequency?resource=download).
