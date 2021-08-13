#ifndef UR_UTILS_H
#define UR_UTILS_H

#include <string>

namespace urscanner
{

bool IsMultiPartUr(const std::string&);
bool IsSinglePartUr(const std::string&);

int GetMultipartUrSeqNum(const std::string&);

struct UrCompare
{
    bool operator() (const std::string& lhs, const std::string& rhs) const
    {
        if (!IsMultiPartUr(lhs) || !IsMultiPartUr(rhs))
        {
            return lhs < rhs;
        }
        return GetMultipartUrSeqNum(lhs) < GetMultipartUrSeqNum(rhs);
    }
};

} // namespace urscanner

#endif // UR_UTILS_H
