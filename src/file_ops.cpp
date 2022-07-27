#include "file_ops.h"

// Helper function that inserts a suffix into a filepath infront of the extension
// Takes const string& for 2 reasons: easier to concatenate strings and filepaths
// tend to be taken as const string& in many C++ functions.
// Example: InsertFilePathSuffix("a.txt", "b") -> "ab.txt"
std::string InsertFilePathSuffix(const std::string &fp, const std::string &suffix)
{
    auto idx{fp.find_last_of('.')};
    return fp.substr(0, idx) + suffix + fp.substr(idx);
}