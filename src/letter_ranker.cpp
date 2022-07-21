#include "letter_ranker.h"
#include <fstream>
#include "exceptions.h"
#include <vector>
#include <algorithm>

LetterRanker::LetterRanker(){};

void LetterRanker::SetUp(const std::string &temp_fp)
{
    std::unordered_map<char, int> counts;
    std::ifstream temp_file(temp_fp, std::ios_base::in);

    if (!temp_file.is_open())
    {
        throw WordleSolverException("Could not open " + temp_fp + " for reading");
    }

    std::string word;
    while (temp_file.good())
    {
        std::getline(temp_file, word);
        for (const auto &c : word)
        {
            counts[c] += 1;
        }
    }
    temp_file.close();

    std::vector<char> keys;
    for (const auto &p : counts)
    {
        keys.push_back(p.first);
    }
    std::sort(keys.begin(), keys.end(), [&counts](char k1, char k2)
              { return counts[k1] >= counts[k2]; });

    ranking.clear();
    for (size_t i{0}; i < keys.size(); i++)
    {
        ranking[keys[i]] = i + 1;
    }
}

int LetterRanker::Rank(std::string_view word)
{
    int rank{0};
    for (const auto &c : word)
    {
        rank += ranking[c];
    }
    return rank;
}

std::string LetterRanker::Name()
{
    return "LetterRanker";
}