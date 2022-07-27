#include "restricted_duplicate_penalty_letter_ranker.h"

RestrictedDuplicatePenaltyLetterRanker::RestrictedDuplicatePenaltyLetterRanker(int p, unsigned short ng): DuplicatePenaltyLetterRanker(p), num_guesses(ng) {}

int RestrictedDuplicatePenaltyLetterRanker::Rank(std::string_view word, unsigned short guess) const {
    return (guess <= num_guesses) ? DuplicatePenaltyLetterRanker::Rank(word, guess) : LetterRanker::Rank(word, guess);
}

std::string RestrictedDuplicatePenaltyLetterRanker::Name() const {
    return "RestrictedDuplicatePenaltyLetterRanker";
}