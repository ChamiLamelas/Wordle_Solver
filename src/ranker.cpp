// Implementation of part of AbstractRanker
// Author: Chami Lamelas
// Date: Summer 2022

#include "ranker.h"

AbstractRanker::AbstractRanker(std::string_view n) : name(n) {}

AbstractRanker::~AbstractRanker() {}

std::string AbstractRanker::GetName() const
{
    return name;
}

std::string AbstractRanker::GetDebugInfo() const
{
    return "";
}
