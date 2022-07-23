// Implementation of evaluation header
// Author: Chami Lamelas
// Date: Summer 2022

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
#include "exceptions.h"

std::string GetFeedback(std::string_view guess, std::string_view word)
{
    // Feedback that will be returned
    std::string feedback(5, 'b');

    // Count of each letter in word that is missing from guess
    std::unordered_map<char, int> counts;

    // Loop over word, calculate letter counts and derive green feedback
    // characters (easier than yellow and black)
    for (auto witr{word.cbegin()}; witr != word.cend(); witr++)
    {
        counts[*witr]++;
        auto idx{std::distance(word.cbegin(), witr)};
        if (guess[idx] == *witr)
        {
            feedback[idx] = 'g';
            // Guess has found letter at idx
            counts[*witr]--;
        }
    }

    // Pass over guess to identify yellow and black letters
    for (auto gitr{guess.cbegin()}; gitr != guess.cend(); gitr++)
    {
        auto idx{std::distance(guess.cbegin(), gitr)};
        // If current guess character is not equal to word[idx] but
        // there is an instance of it that has not been found elsewhere
        // in word, mark feedback yellow and decrement that current
        // guess character accounts for one of missing instances of
        // it in the word
        if (*gitr != word[idx] && counts[*gitr] > 0)
        {
            counts[*gitr]--;
            feedback[idx] = 'y';
        }
        // If guess character is equal to word[idx], don't do anything
        // because feedback[idx] should remain green. If the guess
        // character is != to word[idx] but the count is 0, leave
        // feedback[idx] black. This ensures that the following examples
        // are treated properly: guess "again" for word "bland" has feedback
        // "bbgby", guess "aroma" for word "bland" has feedback "ybbbb",
        // guess "again" for word "aroma" has feedback "gbybb".
    }

    return feedback;
}

short Evaluate(WordleSolver &solver, std::string_view word)
{   
    // Current guess, feedback
    std::string guess;
    std::string feedback;

    // num_guesses is the number of guesses made up to this point
    for (auto num_guesses{0}; num_guesses < 6; num_guesses++)
    {
        // 0 guesses have been made previously, make initial guess, otherwise
        // use feedback from previous guess
        guess = (num_guesses == 0) ? solver.Guess() : solver.Guess(feedback);
        feedback = GetFeedback(guess, word);
        if (feedback == "yyyyy")
        {
            return num_guesses;
        }
    }
    return FAILED;
}

void GetStatistics(const std::vector<short> &guess_counts, double &mean, double &std_dev, size_t &fail_count)
{
    // Set fail count to 0 in case user passes something else
    fail_count = 0;

    // Iterate over guess counts and calculate non-FAILED guess count total and
    // number of FAILED counts
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

    // Compute mean of non-FAILED counts
    mean = counts_total / (guess_counts.size() - fail_count);

    // Compute total of squared deviations from the mean of non-FAILED elements
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

void GridEvaluate(const std::vector<std::string> &dictionary_fps, const std::vector<BaseRanker *> &rankers, const std::string &words_fp, const std::string &output_fp)
{
    // Read-only file stream to words
    std::ifstream words_file(words_fp, std::ios_base::in);
    if (!words_file.is_open())
    {
        throw WordleSolverException("Could not open words file to read");
    }

    // Write-only file stream to output
    std::ofstream output_file(output_fp, std::ios_base::out);
    if (!output_file.is_open())
    {
        throw WordleSolverException("Could not open output file to write");
    }

    // Read words into vector
    std::vector<std::string> words;
    std::string current_line;
    while (words_file.good())
    {
        std::getline(words_file, current_line);
        words.push_back(current_line);
    }
    words_file.close();

    // Write titles to output file
    output_file << std::left << std::setw(40) << "Dictionary" << std::setw(15) << "Ranker" << std::setw(10) << "Mean"
                << std::setw(10) << "SD" << std::setw(10) << "Failures" << std::endl;

    // 6 decimals shown with all output (including integers stored in floats)
    output_file << std::fixed;

    std::vector<short> guess_counts(words.size());
    double mean{0};
    double std_dev{0};
    size_t fail_count{0};

    // Iterate over dictionaries, ranking schemes and construct a WordleSolver for each
    for (auto dfp_itr{dictionary_fps.cbegin()}; dfp_itr != dictionary_fps.cend(); dfp_itr++)
    {
        for (auto rkr_itr{rankers.cbegin()}; rkr_itr != rankers.cend(); rkr_itr++)
        {
            WordleSolver solver(*dfp_itr, *rkr_itr);

            // With constructed solver, iterate over words and get guess count for
            // that word with Evaluate() 
            for (auto w_itr{words.cbegin()}; w_itr != words.cend(); w_itr++)
            {
                auto idx{std::distance(words.cbegin(), w_itr)};
                guess_counts[idx] = Evaluate(solver, *w_itr);
            }

            // Compute statistics and write to output file
            GetStatistics(guess_counts, mean, std_dev, fail_count);
            output_file << std::left << std::setw(40) << *dfp_itr << std::setw(15) << *rkr_itr << std::setw(10)
                        << std::right << mean << std::setw(10) << std_dev << std::setw(10) << fail_count << std::endl;
        }
    }

    output_file.close();
}