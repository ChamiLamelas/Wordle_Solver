// Implementation for position_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "position_ranker.h"
#include <fstream>
#include "misc.h"

PositionRanker::PositionRanker() : AbstractRanker("PositionRanker") {}
PositionRanker::PositionRanker(std::string_view name) : AbstractRanker(name) {}
void PositionRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    std::ifstream eligible_file(eligible_fp, std::ios_base::in);
    if (!eligible_file.is_open())
    {
        throw WordleSolverException("Could not open " + eligible_fp + " for reading");
    }

    // Stores the current word read from file
    std::string word;

    // Read over each letter in each eligible word and increase its count after resetting counts.
    for (auto m : word_counts)
    {
        m.clear();
    }

    while (eligible_file.good())
    {
        std::getline(eligible_file, word);
        for (auto i{0}; i < word.size(); i++)
        {
            word_counts[i][word[i]]++;
        }
    }
    eligible_file.close();

    for (auto i{0}; i < 5; i++)
    {
        CountsToRanks(word_counts[i], rankings[i], letters[i]);
    }
}

int PositionRanker::Rank(std::string_view word, unsigned short guess) const
{
    int rank{0};
    for (auto i{0}; i < word.size(); i++)
    {
        rank += rankings[i].find(word[i])->second;
    }
    return rank;
}

std::string PositionRanker::GetDebugInfo() const
{
    std::string debug_info;
    debug_info.reserve(26 * 50);
    for (auto i{0}; i < 5; i++)
    {
        debug_info += "Index ";
        debug_info += std::to_string(i);
        debug_info += "\n";

        for (auto l : letters[i])
        {
            debug_info += l;
            debug_info += ": ";
            debug_info += std::to_string(rankings[i].find(l)->second);
            debug_info += " ";
            debug_info += std::to_string(word_counts[i].find(l)->second);
            debug_info += "\n";
        }
    }
    return debug_info;
}