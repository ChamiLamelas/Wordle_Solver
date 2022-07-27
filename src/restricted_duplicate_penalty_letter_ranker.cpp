// Implementation for restricted_duplicate_penalty_letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "restricted_duplicate_penalty_letter_ranker.h"

RestrictedDuplicatePenaltyLetterRanker::RestrictedDuplicatePenaltyLetterRanker(int p, unsigned short ng): DuplicatePenaltyLetterRanker("RestrictedDuplicatePenaltyLetterRanker", p), num_guesses(ng) {}

RestrictedDuplicatePenaltyLetterRanker::RestrictedDuplicatePenaltyLetterRanker(std::string_view name, int p, unsigned short ng): DuplicatePenaltyLetterRanker(name, p), num_guesses(ng) {}

int RestrictedDuplicatePenaltyLetterRanker::Rank(std::string_view word, unsigned short guess) const {
    return (guess <= num_guesses) ? DuplicatePenaltyLetterRanker::Rank(word, guess) : LetterRanker::Rank(word, guess);
}