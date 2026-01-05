#pragma once

NS_BEGIN

CStr8 UrlEscape8(PCSTR szUrl);
void UrlEscape8(CStr8 &strUrl);

CStr8 UrlUnescape8(PCSTR szUrl);
void UrlUnescape8(CStr8 &strUrl);

CStrW UrlEscapeW(PCWSTR szUrl);
void UrlEscapeW(CStrW &strUrl);

CStrW UrlUnescapeW(PCWSTR szUrl);
void UrlUnescapeW(CStrW &strUrl);

CStr8 UrlEncode8(PCSTR szUrl);
void UrlEncode8(CStr8 &strUrl);

CStr8 UrlDecode8(PCSTR szUrl);
void UrlDecode8(CStr8 &strUrl);

CStrW UrlEncodeW(PCWSTR szUrl);
void UrlEncodeW(CStrW &strUrl);

CStrW UrlDecodeW(PCWSTR szUrl);
void UrlDecodeW(CStrW &strUrl);

#ifdef _WIN32
#define UrlEscape	UrlEscapeW
#define UrlUnescape	UrlUnescapeW
#define UrlEncode	UrlEncodeW
#define UrlDecode	UrlDecodeW
#else
#define UrlEscape	UrlEscape8
#define UrlUnescape	UrlUnescape8
#define UrlEncode	UrlEncode8
#define UrlDecode	UrlDecode8
#endif

NS_END
