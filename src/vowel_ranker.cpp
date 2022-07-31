// Implementation for vowel_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "vowel_ranker.h"

VowelRanker::VowelRanker(std::string_view name, AbstractRanker *r, int cp) : VowelRanker(name, r, cp, 7) {}
VowelRanker::VowelRanker(std::string_view name, AbstractRanker *r, int cp, unsigned short ng) : AbstractRanker(name), ranker(r), consonant_penalty(cp), num_guesses(ng) {}

void VowelRanker::SetUp(const std::string &eligible_fp, unsigned short guess)
{
    ranker->SetUp(eligible_fp, guess);
}

int VowelRanker::Rank(std::string_view word, unsigned short guess) const
{
    if (guess > num_guesses)
    {
        return ranker->Rank(word, guess);
    }

    auto consonants{0};
    for (auto c : word)
    {
        consonants += VOWELS.find(c) == VOWELS.end();
    }
    return ranker->Rank(word, guess) + (consonant_penalty * consonants);
}

std::string VowelRanker::GetDebugInfo() const
{
    return ranker->GetDebugInfo();
}