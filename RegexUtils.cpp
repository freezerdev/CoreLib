#include "Base.h"
#include "RegexUtils.h"
#include <regex>

NS_BEGIN

//#################################################################################################
bool RegexSearch8(PCSTR szTarget, PCSTR szRegexPattern, const bool bCaseInsensitive, std::vector<CStr8> *pvecResults)
{
	bool bResult = false;

	if(szRegexPattern == nullptr)
		return bResult;

	auto flags = std::regex_constants::ECMAScript;
	if(bCaseInsensitive)
		ADD_FLAG(flags, std::regex::icase);

	try
	{
		std::regex rgx(szRegexPattern, flags);

		if(pvecResults)
		{
			pvecResults->clear();

			PCSTR szScan = szTarget;
			std::cmatch matches;
			while(std::regex_search(szScan, matches, rgx))
			{
				bResult = true;

				for(const auto &itr : matches)
					pvecResults->emplace_back(itr);

				if(StringCompare8(szScan, StringGetLength(szScan), matches.suffix().first, StringGetLength(matches.suffix().first)) == 0)
					break;	// Do not get caught in an infinite loop in the event the target string does not change

				szScan = matches.suffix().first;
			}
		}
		else
			bResult = std::regex_search(szTarget, rgx);
	}
	catch(const std::regex_error &e)
	{	// Catch any regex exceptions
		UNUSED(e);
		Assert(false);
		bResult = false;
		if(pvecResults)
			pvecResults->clear();
	}

	return bResult;
}

//#################################################################################################
bool RegexSearchW(PCWSTR szTarget, PCWSTR szRegexPattern, const bool bCaseInsensitive, std::vector<CStrW> *pvecResults)
{
	bool bResult = false;

	if(szRegexPattern == nullptr)
		return bResult;

	auto flags = std::regex_constants::ECMAScript;
	if(bCaseInsensitive)
		ADD_FLAG(flags, std::regex::icase);

	try
	{
		std::wregex rgx(szRegexPattern, flags);

		if(pvecResults)
		{
			pvecResults->clear();

			PCWSTR szScan = szTarget;
			std::wcmatch matches;
			while(std::regex_search(szScan, matches, rgx))
			{
				bResult = true;

				for(const auto &itr : matches)
					pvecResults->emplace_back(itr);

				if(StringCompareW(szScan, StringGetLength(szScan), matches.suffix().first, StringGetLength(matches.suffix().first)) == 0)
					break;	// Do not get caught in an infinite loop in the event the target string does not change

				szScan = matches.suffix().first;
			}
		}
		else
			bResult = std::regex_search(szTarget, rgx);
	}
	catch(const std::regex_error &e)
	{	// Catch any regex exceptions
		UNUSED(e);
		Assert(false);
		bResult = false;
		if(pvecResults)
			pvecResults->clear();
	}

	return bResult;
}

//#################################################################################################
bool RegexMatch8(PCSTR szTarget, PCSTR szRegexPattern, const bool bCaseInsensitive, std::vector<CStr8> *pvecResults)
{
	bool bResult = false;

	if(szRegexPattern == nullptr)
		return bResult;

	auto flags = std::regex_constants::ECMAScript;
	if(bCaseInsensitive)
		ADD_FLAG(flags, std::regex::icase);

	try
	{
		std::regex rgx(szRegexPattern, flags);

		if(pvecResults)
		{
			pvecResults->clear();

			std::cmatch matches;
			bResult = std::regex_match(szTarget, matches, rgx);
			if(bResult)
			{
				for(const auto &itr : matches)
					pvecResults->emplace_back(itr);
			}
		}
		else
			bResult = std::regex_match(szTarget, rgx);
	}
	catch(const std::regex_error &e)
	{	// Catch any regex exceptions
		UNUSED(e);
		Assert(false);
		bResult = false;
		if(pvecResults)
			pvecResults->clear();
	}

	return bResult;
}

//#################################################################################################
bool RegexMatchW(PCWSTR szTarget, PCWSTR szRegexPattern, const bool bCaseInsensitive, std::vector<CStrW> *pvecResults)
{
	bool bResult = false;

	if(szRegexPattern == nullptr)
		return bResult;

	auto flags = std::regex_constants::ECMAScript;
	if(bCaseInsensitive)
		ADD_FLAG(flags, std::regex::icase);

	try
	{
		std::wregex rgx(szRegexPattern, flags);

		if(pvecResults)
		{
			pvecResults->clear();

			std::wcmatch matches;
			bResult = std::regex_match(szTarget, matches, rgx);
			if(bResult)
			{
				for(const auto &itr : matches)
					pvecResults->emplace_back(itr);
			}
		}
		else
			bResult = std::regex_match(szTarget, rgx);
	}
	catch(const std::regex_error &e)
	{	// Catch any regex exceptions
		UNUSED(e);
		Assert(false);
		bResult = false;
		if(pvecResults)
			pvecResults->clear();
	}

	return bResult;
}

NS_END
