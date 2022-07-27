// Implementation of random_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "random_ranker.h"
#include <ctime>
#include <string>

RandomRanker::RandomRanker(): AbstractRanker("RandomRanker")
{
    srand(time(NULL));
}

RandomRanker::RandomRanker(unsigned int seed): AbstractRanker("RandomRanker")
{
    srand(seed);
}

RandomRanker::RandomRanker(std::string_view name, unsigned int seed): AbstractRanker(name)
{
    srand(seed);
}

void RandomRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    // Have nothing to do
}

int RandomRanker::Rank(std::string_view word, unsigned short guess) const
{
    return rand();
}