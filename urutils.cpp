#include "urutils.h"

#include <regex>

namespace urscanner
{

bool IsMultiPartUr(const std::string& str)
{
    const auto multiPartUrRegex = std::regex("ur:[a-zA-Z0-9-]+/[1-9][0-9]*-[1-9][0-9]*/[a-z]+");
    return std::regex_match(str, multiPartUrRegex);
}

bool IsSinglePartUr(const std::string& str)
{
    const auto singlePartUrRegex = std::regex("ur:[a-zA-Z0-9-]+/[a-z]+");
    return std::regex_match(str, singlePartUrRegex);
}

int GetMultipartUrSeqNum(const std::string& str)
{
    const auto multiPartUrRegexWithSeqNumCapture = std::regex("ur:[a-zA-Z0-9-]+/([1-9][0-9]*)-[1-9][0-9]*/[a-z]+");
    std::smatch match;
    if (std::regex_match(str, match, multiPartUrRegexWithSeqNumCapture))
    {
        return std::stoi(match.str(1));
    }
    return -1;
}

} // namespace urscanner
