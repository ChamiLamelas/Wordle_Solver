// Implementation for vowel_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "vowel_ranker.h"
#include <algorithm>

VowelRanker::VowelRanker(AbstractRanker *r, int cp) : VowelRanker(r, cp, 7) {}
VowelRanker::VowelRanker(AbstractRanker *r, int cp, unsigned short ng) : VowelRanker("VowelRanker(" + r->GetName() + "," + std::to_string(cp) + "," + std::to_string(ng) + ")", r, cp, ng) {}
VowelRanker::VowelRanker(std::string_view name, AbstractRanker *r, int cp) : VowelRanker(name, r, cp, 7) {}
VowelRanker::VowelRanker(std::string_view name, AbstractRanker *r, int cp, unsigned short ng) : AbstractRanker(name), ranker(r), consonant_penalty(cp), num_guesses(ng) {}

void VowelRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    curr_guess = guess;
    ranker->SetUp(eligible_fp, guess, feedback);
}

int VowelRanker::Rank(std::string_view word) const
{
    if (curr_guess > num_guesses)
    {
        return ranker->Rank(word);
    }

    auto consonants{0};
    for (auto c : word)
    {
        consonants += VOWELS.find(c) == VOWELS.end();
    }
    return ranker->Rank(word) + (consonant_penalty * consonants);
}

std::string VowelRanker::GetDebugInfo() const
{
    return ranker->GetDebugInfo();
}


ProgressVowelRanker::ProgressVowelRanker(AbstractRanker *r, int cp) : ProgressVowelRanker(r, cp, 7) {}
ProgressVowelRanker::ProgressVowelRanker(AbstractRanker *r, int cp, unsigned short nf) : ProgressVowelRanker("ProgressVowelRanker(" + r->GetName() + "," + std::to_string(cp) + "," + std::to_string(nf) + ")", r, cp, nf) {}
ProgressVowelRanker::ProgressVowelRanker(std::string_view name, AbstractRanker *r, int cp) : ProgressVowelRanker(name, r, cp, 7) {}
ProgressVowelRanker::ProgressVowelRanker(std::string_view name, AbstractRanker *r, int cp, unsigned short nf) : AbstractRanker(name), ranker(r), consonant_penalty(cp), num_found(nf) {}

void ProgressVowelRanker::SetUp(const std::string &eligible_fp, unsigned short guess, std::string_view feedback)
{
    curr_found = feedback.size() - std::count(feedback.cbegin(), feedback.cend(), 'b');;
    ranker->SetUp(eligible_fp, guess, feedback);
}

int ProgressVowelRanker::Rank(std::string_view word) const
{
    if (curr_found >= num_found)
    {
        return ranker->Rank(word);
    }

    auto consonants{0};
    for (auto c : word)
    {
        consonants += VOWELS.find(c) == VOWELS.end();
    }
    return ranker->Rank(word) + (consonant_penalty * consonants);
}

std::string ProgressVowelRanker::GetDebugInfo() const
{
    return ranker->GetDebugInfo();
}