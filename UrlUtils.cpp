#include "Base.h"
#include "UrlUtils.h"

NS_BEGIN

//#################################################################################################
CStr8 UrlEscape8(PCSTR szUrl)
{
	CStr8 strEscapedUrl(szUrl);
	UrlEscape8(strEscapedUrl);

	return strEscapedUrl;
}

//#################################################################################################
void UrlEscape8(CStr8 &strUrl)
{
	CStr8 strReplace;
	for(size_t nPos = 0; nPos < strUrl.GetLength(); ++nPos)
	{
		unsigned char ch = strUrl.GetAt(nPos);

		if(ch <= ')' ||
			ch == ',' ||
			(ch >= ':' && ch <= '?') ||
			(ch >= '[' && ch <= '^') ||
			ch == '`' ||
			ch >= '{')
		{
			strReplace.Printf("%%%02X", ch);
			strUrl.Delete(nPos);
			strUrl.Insert(nPos, strReplace);
			nPos += strReplace.GetLength() - 1;
		}
	}
}

//#################################################################################################
CStr8 UrlUnescape8(PCSTR szUrl)
{
	CStr8 strUnescapedUrl(szUrl);
	UrlUnescape8(strUnescapedUrl);

	return strUnescapedUrl;
}

//#################################################################################################
void UrlUnescape8(CStr8 &strUrl)
{
	size_t nPos;
	size_t nStart = 0;
	while((nPos = strUrl.Find('%', nStart)) != CStr8::not_found)
	{
		char ch1 = strUrl.GetAt(nPos + 1);
		char ch2 = strUrl.GetAt(nPos + 2);

		if(IsHexChar8(ch1) && IsHexChar8(ch2))
		{
			char chReplace = StringToInteger8<char>(strUrl.GetMid(nPos + 1, 2), 16);
			strUrl.Delete(nPos, 3);
			strUrl.Insert(nPos, chReplace);
		}

		nStart = nPos + 1;
	}
}

//#################################################################################################
CStrW UrlEscapeW(PCWSTR szUrl)
{
	CStrW strEscapedUrl(szUrl);
	UrlEscapeW(strEscapedUrl);

	return strEscapedUrl;
}

//#################################################################################################
void UrlEscapeW(CStrW &strUrl)
{
	CStrW strReplace;
	for(size_t nPos = 0; nPos < strUrl.GetLength(); ++nPos)
	{
		wchar_t ch = strUrl.GetAt(nPos);

		if(ch <= L')' ||
			ch == L',' ||
			(ch >= L':' && ch <= L'?') ||
			(ch >= L'[' && ch <= L'^') ||
			ch == L'`' ||
			(ch >= L'{' && ch <= 255))
		{
			strReplace.Printf(L"%%%02X", ch);
			strUrl.Delete(nPos);
			strUrl.Insert(nPos, strReplace);
			nPos += strReplace.GetLength() - 1;
		}
		else if(ch > 255)
		{
			strReplace.Printf(L"%%u%04X", ch);
			strUrl.Delete(nPos);
			strUrl.Insert(nPos, strReplace);
			nPos += strReplace.GetLength() - 1;
		}
	}
}

//#################################################################################################
CStrW UrlUnescapeW(PCWSTR szUrl)
{
	CStrW strUnescapedUrl(szUrl);
	UrlUnescapeW(strUnescapedUrl);

	return strUnescapedUrl;
}

//#################################################################################################
void UrlUnescapeW(CStrW &strUrl)
{
	size_t nPos;
	size_t nStart = 0;
	while((nPos = strUrl.Find(L'%', nStart)) != CStrW::not_found)
	{
		wchar_t ch1 = strUrl.GetAt(nPos + 1);
		wchar_t ch2 = strUrl.GetAt(nPos + 2);
		wchar_t ch3 = strUrl.GetAt(nPos + 3);
		wchar_t ch4 = strUrl.GetAt(nPos + 4);
		wchar_t ch5 = strUrl.GetAt(nPos + 5);

		if(IsHexCharW(ch1) && IsHexCharW(ch2))
		{
			wchar_t chReplace = StringToIntegerW<wchar_t>(strUrl.GetMid(nPos + 1, 2), 16);
			strUrl.Delete(nPos, 3);
			strUrl.Insert(nPos, chReplace);
		}
		else if(ch1 == L'u' && IsHexCharW(ch2) && IsHexCharW(ch3) && IsHexCharW(ch4) && IsHexCharW(ch5))
		{
			wchar_t chReplace = StringToIntegerW<wchar_t>(strUrl.GetMid(nPos + 2, 4), 16);
			strUrl.Delete(nPos, 6);
			strUrl.Insert(nPos, chReplace);
		}

		nStart = nPos + 1;
	}
}

//#################################################################################################
CStr8 UrlEncode8(PCSTR szUrl)
{
	CStr8 strEncodedUrl(szUrl);
	UrlEncode8(strEncodedUrl);

	return strEncodedUrl;
}

//#################################################################################################
void UrlEncode8(CStr8 &strUrl)
{
	CStr8 strReplace;
	for(size_t nPos = 0; nPos < strUrl.GetLength(); ++nPos)
	{
		unsigned char ch = strUrl.GetAt(nPos);

		if(ch <= ' ' ||
			(ch >= '"' && ch <= '&') ||
			ch == '+' ||
			ch == ',' ||
			ch == '/' ||
			(ch >= ':' && ch <= '@') ||
			(ch >= '[' && ch <= '^') ||
			ch == '`' ||
			(ch >= '{' && ch <= '}') ||
			ch >= 127)
		{
			strReplace.Printf("%%%02X", ch);
			strUrl.Delete(nPos);
			strUrl.Insert(nPos, strReplace);
			nPos += strReplace.GetLength() - 1;
		}
	}
}

//#################################################################################################
CStr8 UrlDecode8(PCSTR szUrl)
{
	CStr8 strDecodedUrl(szUrl);
	UrlDecode8(strDecodedUrl);

	return strDecodedUrl;
}

//#################################################################################################
void UrlDecode8(CStr8 &strUrl)
{
	size_t nPos;
	size_t nStart = 0;
	while((nPos = strUrl.Find('%', nStart)) != CStr8::not_found)
	{
		char ch1 = strUrl.GetAt(nPos + 1);
		char ch2 = strUrl.GetAt(nPos + 2);

		if(IsHexChar8(ch1) && IsHexChar8(ch2))
		{
			char chReplace = StringToInteger8<char>(strUrl.GetMid(nPos + 1, 2), 16);
			strUrl.Delete(nPos, 3);
			strUrl.Insert(nPos, chReplace);
		}

		nStart = nPos + 1;
	}
}

//#################################################################################################
CStrW UrlEncodeW(PCWSTR szUrl)
{	// URL encoding is based on UTF8 character codes, so convert the URL to UTF8 and back
	CStr8 strEncodedUrl(szUrl);
	UrlEncode8(strEncodedUrl);

	return strEncodedUrl.AsWide();
}

//#################################################################################################
void UrlEncodeW(CStrW &strUrl)
{	// URL encoding is based on UTF8 character codes, so convert the URL to UTF8 and back
	CStr8 strEncodedUrl(strUrl);
	UrlEncode8(strEncodedUrl);
	strUrl = strEncodedUrl;
}

//#################################################################################################
CStrW UrlDecodeW(PCWSTR szUrl)
{	// URL encoding is based on UTF8 character codes, so convert the URL to UTF8 and back
	CStr8 strDecodedUrl(szUrl);
	UrlDecode8(strDecodedUrl);

	return strDecodedUrl.AsWide();
}

//#################################################################################################
void UrlDecodeW(CStrW &strUrl)
{	// URL encoding is based on UTF8 character codes, so convert the URL to UTF8 and back
	CStr8 strDecodedUrl(strUrl);
	UrlDecode8(strDecodedUrl);
	strUrl = strDecodedUrl;
}

NS_END
