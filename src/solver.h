#ifndef SOLVER_H
#define SOLVER_H

#include <exception>
#include "ranker.h"
#include <string>
#include <string_view>
#include <set>

class WordleSolver
{
private:
    short num_guesses;
    BaseRanker *ranker;
    std::set<short> found_indices;
    std::string dataset_fp;
    std::string temp_fp;
    std::string prev_guess;

    class Private;

    static std::string PLACEHOLDER;
    static std::string TEMP_FP_SUFFIX;

public:
    WordleSolver(std::string_view d_fp, BaseRanker *r);
    std::string Guess();
    std::string Guess(std::string_view feedback);
};

#endif