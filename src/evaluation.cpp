#include <string>
#include <unordered_map>
#include <iterator>
#include "ranker.h"
#include <vector>
#include "solver.h"
#include "evaluation.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

EvaluationFailureException::EvaluationFailureException(std::string_view r) : reason(r) {}

const char *EvaluationFailureException::what() const throw()
{
    return reason.c_str();
}

std::string GetFeedback(std::string_view guess, std::string_view word)
{
    std::string feedback(5, 'b');
    std::unordered_map<char, int> counts;
    for (auto witr{word.cbegin()}; witr != word.cend(); witr++)
    {
        counts[*witr]++;
        auto idx{std::distance(word.cbegin(), witr)};
        if (guess[idx] == *witr)
        {
            feedback[idx] = 'g';
            counts[*witr]--;
        }
    }
    for (auto gitr{guess.cbegin()}; gitr != guess.cend(); gitr++)
    {
        auto idx{std::distance(guess.cbegin(), gitr)};
        if (*gitr != word[idx] && counts[*gitr] > 0)
        {
            counts[*gitr]--;
            feedback[idx] = 'y';
        }
    }

    return feedback;
}

short Evaluate(WordleSolver &solver, std::string_view word)
{
    std::string guess{solver.Guess()};
    std::string feedback;
    for (auto num_guesses{1}; num_guesses <= 6; num_guesses++)
    {
        feedback = GetFeedback(guess, word);
        if (feedback == "yyyyy")
        {
            return num_guesses;
        }
        solver.Guess(feedback);
    }
    return FAILED;
}

void GetStatistics(const std::vector<short> &guess_counts, double &mean, double &std_dev, size_t &fail_count)
{
    fail_count = 0;
    size_t counts_total{0};
    for (auto citr{guess_counts.cbegin()}; citr != guess_counts.cend(); citr++)
    {
        if (*citr == FAILED)
        {
            fail_count++;
        }
        else
        {
            counts_total += *citr;
        }
    }
    mean = counts_total / (guess_counts.size() - fail_count);
    size_t dev_sq_total{0};
    for (auto citr{guess_counts.cbegin()}; citr != guess_counts.cend(); citr++)
    {
        if (*citr != FAILED)
        {
            dev_sq_total += std::pow(*citr - mean, 2);
        }
    }
    std_dev = std::sqrt(dev_sq_total / (guess_counts.size() - fail_count));
}

void EvaluateDataset(const std::vector<std::string> &dictionary_fps, const std::vector<BaseRanker *> rankers, const std::string &words_fp, const std::string &output_fp)
{
    std::ifstream words_file(words_fp, std::ios_base::in);
    if (!words_file.is_open())
    {
        throw EvaluationFailureException("Could not open words file to read");
    }

    std::ofstream output_file(output_fp, std::ios_base::out);
    if (!output_file.is_open())
    {
        throw EvaluationFailureException("Could not open output file to write");
    }

    std::vector<std::string> words;
    std::string current_line;
    while (words_file.good())
    {
        std::getline(words_file, current_line);
        words.push_back(current_line);
    }
    words_file.close();

    output_file << std::left << std::setw(40) << "Dictionary" << std::setw(15) << "Ranker" << std::setw(10) << "Mean"
                << std::setw(10) << "SD" << std::setw(10) << "Failures" << std::endl;

    output_file << std::fixed << std::showpoint;

    std::vector<short> guess_counts(words.size());
    double mean{0};
    double std_dev{0};
    size_t fail_count{0};
    for (auto dfp_itr{dictionary_fps.cbegin()}; dfp_itr != dictionary_fps.cend(); dfp_itr++)
    {
        for (auto rkr_itr{rankers.cbegin()}; rkr_itr != rankers.cend(); rkr_itr++)
        {
            WordleSolver solver(*dfp_itr, *rkr_itr);
            for (auto w_itr{words.cbegin()}; w_itr != words.cend(); w_itr++)
            {
                auto idx{std::distance(words.cbegin(), w_itr)};
                guess_counts[idx] = Evaluate(solver, *w_itr);
            }
            GetStatistics(guess_counts, mean, std_dev, fail_count);
            output_file << std::left << std::setw(40) << *dfp_itr << std::setw(15) << *rkr_itr << std::setw(10)
                        << std::right << mean << std::setw(10) << std_dev << std::setw(10) << fail_count << std::endl;
        }
    }

    output_file.close();
}