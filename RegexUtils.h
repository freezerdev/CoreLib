#pragma once

#include <vector>

NS_BEGIN

// Returns true if even a partial match of pattern inside the target string
bool RegexSearch8(PCSTR szTarget, PCSTR szRegexPattern, const bool bCaseInsensitive = true, std::vector<CStr8> *pvecResults = nullptr);
bool RegexSearchW(PCWSTR szTarget, PCWSTR szRegexPattern, const bool bCaseInsensitive = true, std::vector<CStrW> *pvecResults = nullptr);

// Returns true if the entire pattern matches the target string
bool RegexMatch8(PCSTR szTarget, PCSTR szRegexPattern, const bool bCaseInsensitive = true, std::vector<CStr8> *pvecResults = nullptr);
bool RegexMatchW(PCWSTR szTarget, PCWSTR szRegexPattern, const bool bCaseInsensitive = true, std::vector<CStrW> *pvecResults = nullptr);


#ifdef _WIN32
#define RegexSearch RegexSearchW
#define RegexMatch RegexMatchW
#else
#define RegexSearch RegexSearch8
#define RegexMatch RegexMatch8
#endif

NS_END
