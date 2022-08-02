/*
Header for miscellaneous utilities used elsewhere.

Author: Chami Lamelas
Date: Summer 2022
*/

#ifndef MISC_H
#define MISC_H

#include <string>
#include <exception>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>

/*
Function that inserts a suffix into a filepath infront of the extension.

Takes const string& over string_view for 2 reasons: easier to concatenate
strings and filepaths tend to be taken as const string& in many C++
functions so a string would be needed ahead of time anyway.

Parameters:
    fp: Filepath
    suffix: Suffix

Returns:
    Filepath with suffix infront of the extension. Example:
    InsertFilePathSuffix("a.txt", "b") -> "ab.txt"
*/
std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix);

/*
WordleSolver exception class.

This is a custom exception class that is used to report errors.
This class is created to distinguish errors in AbstractRanker,
WordleSolver, and the evaluation header from exceptions raised
by C++.
*/
class WordleSolverException final : public std::exception
{
public:
    /*
    Constructs a WordleSolverException.

    Parameters:
        r: Reason for the exception being thrown.
    */
    WordleSolverException(std::string_view r);

    /*
    Specifies why the exception was thrown to a catch block for example.

    Returns:
        Reason for exception being thrown.

    Throws:
        Nothing, customary to not have any exception method throw an exception.
    */
    const char *what() const throw() override;

private:
    /*
    Stores the reason the exception was thrown, defines the C-string returned
    by what().
    */
    std::string reason;
};

/*
Ranks elements based on their counts.

Implementation in header because: https://stackoverflow.com/a/36080

Template:
    KeyType: type of elements

Parameters:
    counts: Map specifying counts of each element (not modified).
    ranks: Map that will hold the elements in counts ranked 1,2, and so on. Rank 1
           element had the highest count. Elements with the same count have the
           same rank.
    keys: Vector that will hold the elements in order of increasing rank.
*/
template <typename KeyType>
void CountsToRanks(const std::unordered_map<KeyType, size_t> &counts, std::unordered_map<KeyType, int> &ranks, std::vector<KeyType> &keys)
{
    keys.clear();
    ranks.clear();

    if (counts.size() == 0)
    {
        return;
    }

    for (const auto &s : counts)
    {
        keys.push_back(s.first);
    }

    // To use sort we have a few requirements: first, iterators of container being sorted
    // must be random access iterators (hence vector created prior), second sort predicate
    // must specify strict weak ordering
    std::sort(keys.begin(), keys.end(), [&counts](const KeyType &key1, const KeyType &key2)
              { return counts.find(key1)->second > counts.find(key2)->second; });

    int curr_rank{1};
    auto curr_score{counts.find(keys[0])->second};
    for (const auto &k : keys)
    {
        if (counts.find(k)->second < curr_score)
        {
            curr_rank++;
            curr_score = counts.find(k)->second;
        }
        ranks[k] = curr_rank;
    }
}

#endif