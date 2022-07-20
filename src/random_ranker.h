#ifndef RANDOM_RANKER_H
#define RANDOM_RANKER_H

#include "ranker.h"

class RandomRanker final : public BaseRanker
{
public:
    RandomRanker();
    RandomRanker(unsigned int seed);
    void SetUp(const std::string &temp_fp) override;
    int Rank(std::string_view word) override;
    std::string Name() override;
};

#endif