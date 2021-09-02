/*
 *  Copyright (c) 2021 Pavel Najman
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to dea in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "urutils.h"

#include <regex>

namespace qurscanner
{

bool IsMultiPartUr(const std::string& str)
{
    const auto multiPartUrRegex = std::regex("[uU][rR]:[a-zA-Z0-9-]+/[1-9][0-9]*-[1-9][0-9]*/[a-zA-Z]+");
    return std::regex_match(str, multiPartUrRegex);
}

bool IsSinglePartUr(const std::string& str)
{
    const auto singlePartUrRegex = std::regex("[uU][rR]:[a-zA-Z0-9-]+/[a-zA-Z]+");
    return std::regex_match(str, singlePartUrRegex);
}

int GetMultipartUrSeqNum(const std::string& str)
{
    const auto multiPartUrRegexWithSeqNumCapture = std::regex("[uU][rR]:[a-zA-Z0-9-]+/([1-9][0-9]*)-[1-9][0-9]*/[a-zA-Z]+");
    std::smatch match;
    if (std::regex_match(str, match, multiPartUrRegexWithSeqNumCapture))
    {
        return std::stoi(match.str(1));
    }
    return -1;
}

} // namespace qurscanner
