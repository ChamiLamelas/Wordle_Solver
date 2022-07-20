#ifndef RANKER_H
#define RANKER_H

#include <string_view>
#include <string>

class BaseRanker
{
public:
    virtual ~BaseRanker(){};
    virtual void SetUp(const std::string &temp_fp) = 0;
    virtual int Rank(std::string_view word) = 0;
    virtual std::string Name() = 0;
};

#endif