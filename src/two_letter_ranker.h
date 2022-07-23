#ifndef TWO_LETTER_RANKER_H
#define TWO_LETTER_RANKER_H

#include "ranker.h"
#include <unordered_map>

class TwoLetterRanker : public BaseRanker
{
public:
    TwoLetterRanker();
    virtual void SetUp(const std::string &eligible_fp) override;
    virtual int Rank(std::string_view word) const override;
    virtual std::string Name() const override;
    virtual int GetCount(std::string_view substr) const;

private:
    std::unordered_map<std::string, int> counts;
    std::unordered_map<std::string, int> ranking;
};

#endif