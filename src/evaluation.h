#ifndef EVALUATION_H
#define EVALUATION_H

#include <string_view>
#include <string>
#include <vector>
#include "solver.h"
#include "ranker.h"

const short FAILED{-1};

class EvaluationFailureException : public std::exception
{
private:
    std::string reason;

public:
    EvaluationFailureException(std::string_view r);
    virtual const char *what() const throw();
};

std::string GetFeedback(std::string_view guess, std::string_view word);
short Evaluate(WordleSolver &solver, std::string_view word);
void GetStatistics(const std::vector<short> &guess_counts, double &mean, double &std_dev, size_t &fail_count);
void EvaluateDataset(const std::vector<std::string> &dictionary_fps, const std::vector<BaseRanker *> rankers, const std::string &words_fp, const std::string &output_fp);

#endif