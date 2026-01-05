#include "Base.h"
#include "TextFile.h"
#include "FileSystemUtils.h"
#include "StrUtils.h"
#include <numeric>

NS_BEGIN

//#################################################################################################
bool CTextFile::Load(PCNSTR szTxtPath)
{
	bool bLoaded = false;

	static constexpr auto BUFFER_SIZE = 8192;

	Empty();

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(szTxtPath, EFM_ExistingReadOnly, hFile) == FW_NO_ERROR)
	{
		auto pBuf = std::make_unique<BYTE[]>(BUFFER_SIZE);
		size_t nBytesRead;

		ERRCODE nErrorCode = FileRead(hFile, pBuf.get(), BUFFER_SIZE, nBytesRead);

		if(nBytesRead >= sizeof(g_Utf16LE_Bom) && std::memcmp(pBuf.get(), g_Utf16LE_Bom, sizeof(g_Utf16LE_Bom)) == 0)
		{	// UTF16 file
			char16_t *szScanFile = (char16_t*)(pBuf.get() + sizeof(g_Utf16LE_Bom));
			CStr16 strLine;

			while(nErrorCode == FW_NO_ERROR && nBytesRead)
			{
				while((size_t)szScanFile - (size_t)pBuf.get() < nBytesRead)
				{
					if(*szScanFile != u'\r' && *szScanFile != u'\n')
						strLine += *szScanFile;
					else
					{
						strLine.Trim();
						if(!strLine.IsEmpty())
						{
							m_vecLines.emplace_back(strLine);
							strLine.Empty();
						}
					}

					++szScanFile;
				}

				nErrorCode = FileRead(hFile, pBuf.get(), BUFFER_SIZE, nBytesRead);
				szScanFile = (char16_t*)pBuf.get();
			}

			if(!strLine.IsEmpty())
				m_vecLines.emplace_back(strLine);
		}
		else
		{	// UTF8 file
			PSTR szScanFile = (PSTR)pBuf.get();
			CStr8 strLine;

			while(nErrorCode == FW_NO_ERROR && nBytesRead)
			{
				while((size_t)szScanFile - (size_t)pBuf.get() < nBytesRead)
				{
					if(*szScanFile != '\r' && *szScanFile != '\n')
						strLine += *szScanFile;
					else
					{
						strLine.Trim();
						if(!strLine.IsEmpty())
						{	// Note, the following move is only successful on MacOS/Linux, on Windows it is still a copy
							m_vecLines.emplace_back(std::move(strLine));
							strLine.Empty();
						}
					}

					++szScanFile;
				}

				nErrorCode = FileRead(hFile, pBuf.get(), BUFFER_SIZE, nBytesRead);
				szScanFile = (PSTR)pBuf.get();
			}

			if(!strLine.IsEmpty())
				m_vecLines.emplace_back(std::move(strLine));
		}

		FileClose(hFile);
		bLoaded = true;
	}

	return bLoaded;
}

//#################################################################################################
bool CTextFile::SaveAs(PCNSTR szTxtPath, const EEncodingType eType) const
{
	bool bSaved = false;

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(szTxtPath, EFM_CreateWriteOnly, hFile) == FW_NO_ERROR)
	{
		size_t nBytesWritten;

		FileSetPosition(hFile, 0, EFS_Begin);

#ifdef _WIN32
		if(eType == EET_Utf16 || eType == EET_Native)
#else
		if(eType == EET_Utf16)
#endif
		{	// UTF16 file
			FileWrite(hFile, g_Utf16LE_Bom, sizeof(g_Utf16LE_Bom), nBytesWritten);

			CStr16 strLine;
			for(const auto &line : m_vecLines)
			{
				strLine = line;
				strLine += g_szEol16;

				FileWrite(hFile, strLine, strLine.GetSize(), nBytesWritten);
			}
		}
		else
		{	// UTF8 file
			CStr8 strLine;
			for(const auto &line : m_vecLines)
			{
				strLine = line;
				strLine += g_szEol8;

				FileWrite(hFile, strLine, strLine.GetSize(), nBytesWritten);
			}
		}

		FileSetEnd(hFile);

		FileClose(hFile);
		bSaved = true;
	}

	return bSaved;
}

//#################################################################################################
void CTextFile::ParseFromString(const CStr8 &strTxt)
{
	Empty();

	size_t nStart = 0;
	CStr8 strLine = strTxt.Tokenize(g_szEol8, nStart);
	while(!strLine.IsEmpty())
	{	// Note, the following move is only successful on MacOS/Linux, on Windows it is still a copy
		m_vecLines.emplace_back(std::move(strLine));
		strLine = strTxt.Tokenize(g_szEol8, nStart);
	}
}

//#################################################################################################
void CTextFile::ParseFromString(const CStrW &strTxt)
{
	Empty();

	size_t nStart = 0;
	CStrW strLine = strTxt.Tokenize(g_szEolW, nStart);
	while(!strLine.IsEmpty())
	{	// Note, the following move is only successful on Windows, on MacOS/Linux it is still a copy
		m_vecLines.emplace_back(std::move(strLine));
		strLine = strTxt.Tokenize(g_szEolW, nStart);
	}
}

//#################################################################################################
void CTextFile::SaveToString(CStr8 &strTxt) const
{
	strTxt.Empty();

	for(const auto &line : m_vecLines)
	{
		strTxt += line;
		strTxt += g_szEol8;
	}
}

//#################################################################################################
void CTextFile::SaveToString(CStrW &strTxt) const
{
	strTxt.Empty();

	for(const auto &line : m_vecLines)
	{
		strTxt += line;
		strTxt += g_szEolW;
	}
}

//#################################################################################################
void CTextFile::Empty(void)
{
	m_vecLines.clear();
}

//#################################################################################################
bool CTextFile::IsEmpty(void) const noexcept
{
	return m_vecLines.empty();
}

//#################################################################################################
size_t CTextFile::GetLineCount(void) const
{
	return m_vecLines.size();
}

//#################################################################################################
void CTextFile::AppendLine(PCNSTR sz)
{
	m_vecLines.emplace_back(sz);
}

//#################################################################################################
void CTextFile::AppendBlankLine(void)
{
	m_vecLines.emplace_back();
}

//#################################################################################################
size_t CTextFile::GetFileSize(const EEncodingType eType) const
{
	size_t nSize = 0;

#ifdef _WIN32
	if(eType == EET_Utf16 || eType == EET_Native)
#else
	if(eType == EET_Utf16)
#endif
	{	// UTF16 file
		const size_t nEolSize = StringGetSize(g_szEol16);
		nSize = std::accumulate(m_vecLines.begin(), m_vecLines.end(), sizeof(g_Utf16LE_Bom), [&](const size_t nTotal, const CStr &str){return nTotal + (str.GetLength() * sizeof(char16_t)) + nEolSize;});
	}
	else
	{	// UTF8 file
		const size_t nEolSize = StringGetSize(g_szEol8);
		nSize = std::accumulate(m_vecLines.begin(), m_vecLines.end(), (size_t)0, [&](const size_t nTotal, const CStr &str){return nTotal + str.GetLength() + nEolSize;});
	}

	return nSize;
}

//#################################################################################################
std::vector<CStr>::const_iterator CTextFile::begin(void) const
{
	return m_vecLines.begin();
}

//#################################################################################################
std::vector<CStr>::const_iterator CTextFile::end(void) const
{
	return m_vecLines.end();
}

//#################################################################################################
std::vector<CStr>::iterator CTextFile::begin(void)
{
	return m_vecLines.begin();
}

//#################################################################################################
std::vector<CStr>::iterator CTextFile::end(void)
{
	return m_vecLines.end();
}

NS_END
