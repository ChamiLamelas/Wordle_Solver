// Implementation for restricted_duplicate_penalty_two_letter_ranker.h
// Author: Chami Lamelas
// Date: Summer 2022

#include "restricted_duplicate_penalty_two_letter_ranker.h"

RestrictedDuplicatePenaltyTwoLetterRanker::RestrictedDuplicatePenaltyTwoLetterRanker(int p, unsigned short ng): DuplicatePenaltyTwoLetterRanker(p), num_guesses(ng) {}

int RestrictedDuplicatePenaltyTwoLetterRanker::Rank(std::string_view word, unsigned short guess) const {
    return (guess <= num_guesses) ? DuplicatePenaltyTwoLetterRanker::Rank(word, guess) : TwoLetterRanker::Rank(word, guess);
}

std::string RestrictedDuplicatePenaltyTwoLetterRanker::Name() const {
    return "RestrictedDuplicatePenaltyTwoLetterRanker";
}

