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

void RandomRanker::SetUp(const string &temp_fp)
{
}

int RandomRanker::Rank(std::string_view word)
{
    return rand();
}

std::string RandomRanker::Name()
{
    return "RandomRanker";
}