// Implementation of random_ranker.h

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

int RandomRanker::Rank(std::string_view word)
{
    return rand();
}

std::string RandomRanker::Name() const
{
    return "RandomRanker";
}