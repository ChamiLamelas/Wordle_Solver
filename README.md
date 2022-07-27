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

Once this has been done, pressing Ctrl+Shift+B builds `wordle_solver.exe` which can be run. 

## Further Development

One can easily add additional ranking schemes provided with the `AbstractRanker` abstract data type defined [here](src/ranker.h) and test them either via user mode or via the evaluation header in `main.cpp`. One could also add additional evaluation metrics.

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