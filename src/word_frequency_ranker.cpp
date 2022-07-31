// Implementation for word_frequency_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "word_frequency_ranker.h"
#include <fstream>
#include "misc.h"
#include <limits>

WordFrequencyRanker::WordFrequencyRanker(AbstractRanker *r, const std::string &freq_fp) : WordFrequencyRanker("WordFrequencyRanker(" + r->GetName() + ",\"" + freq_fp + "\")", r, freq_fp) {}
WordFrequencyRanker::WordFrequencyRanker(std::string_view name, AbstractRanker *r, const std::string &freq_fp) : AbstractRanker(name), ranker(r)
{
    std::ifstream words_file(freq_fp, std::ios_base::in);
    if (!words_file.is_open())
    {
        throw WordleSolverException("Could not open " + freq_fp + " for reading");
    }

    std::string line;
    int curr_rank{0};
    long curr_count{LONG_MAX};
    while (words_file.good())
    {
        std::getline(words_file, line);
        auto idx{line.find(',')};
        long count{std::stol(line.substr(idx + 1))};

        if (count < curr_count)
        {
            curr_rank++;
            curr_count = count;
        }
        ranking[line.substr(0, idx)] = curr_rank;
    }

    words_file.close();
}

void WordFrequencyRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    ranker->SetUp(eligible_fp, guess, feedback);
}

int WordFrequencyRanker::Rank(std::string_view word) const
{
    auto p{ranking.find(std::string(word))};
    return ranker->Rank(word) + ((p == ranking.end()) ? ranking.size() + 1 : p->second);
}

std::string WordFrequencyRanker::GetDebugInfo() const
{
    std::string debug_info;
    debug_info.reserve(ranking.size() * 10);
    for (auto p : ranking)
    {
        debug_info += p.first;
        debug_info += ": ";
        debug_info += std::to_string(p.second);
        debug_info += "\n";
    }

    return debug_info + "\n" + ranker->GetDebugInfo();
}