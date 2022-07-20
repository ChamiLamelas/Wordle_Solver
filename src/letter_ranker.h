#ifndef LETTER_RANKER_H
#define LETTER_RANKER_H

#include "ranker.h"
#include <unordered_map>

class LetterRanker final : public BaseRanker
{
private:
    std::unordered_map<char, int> ranking;

public:
    LetterRanker();
    void SetUp(const std::string &temp_fp) override;
    int Rank(std::string_view word) override;
    std::string Name() override;
};

#endif