// Implementation of random_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "random_ranker.h"
#include <ctime>

RandomRanker::RandomRanker()
{
    srand(time(NULL));
}

RandomRanker::RandomRanker(unsigned int seed)
{
    srand(seed);
}

void RandomRanker::SetUp(const std::string &eligible_fp)
{
    // Have nothing to do
}

int RandomRanker::Rank(std::string_view word) const
{
    return rand();
}

std::string RandomRanker::Name() const
{
    return "RandomRanker";
}