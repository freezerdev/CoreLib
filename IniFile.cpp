#include "Base.h"
#include "IniFile.h"
#include "FileSystemUtils.h"
#include "StrUtils.h"

NS_BEGIN

const NCHAR g_chEqual = _N('=');
const NCHAR g_chOpenBracket = _N('[');
const NCHAR g_chCloseBracket = _N(']');
const NCHAR g_chSemiColon = _N(';');

static PCNSTR g_szIntValue = _N("{0}={1}");
static PCNSTR g_szStrValue = _N("{0}=\"{1}\"");
static PCNSTR g_szEmptyValue = _N("{0}=");

//#################################################################################################
inline char GetFirstNonWhiteSpace(const CStr8 &str)
{
	char ch = g_chNull8;

	for(size_t n = 0; n < str.GetLength(); ++n)
	{
		if(!IsSpace8(str[n]))
		{
			ch = str[n];
			break;
		}
	}

	return ch;
}

//#################################################################################################
inline wchar_t GetFirstNonWhiteSpace(const CStrW &str)
{
	wchar_t ch = g_chNullW;

	for(size_t n = 0; n < str.GetLength(); ++n)
	{
		if(!IsSpaceW(str[n]))
		{
			ch = str[n];
			break;
		}
	}

	return ch;
}

//#################################################################################################
CStr FormatSectionName(const CStr &strSection)
{
	Assert(!strSection.IsEmpty());

	CStr strFormattedSection(strSection);
	strFormattedSection.Trim();

	if(strFormattedSection.GetFirst() != g_chOpenBracket)
		strFormattedSection.Prepend(g_chOpenBracket);
	if(strFormattedSection.GetLast() != g_chCloseBracket)
		strFormattedSection.Append(g_chCloseBracket);

	return strFormattedSection;
}

//#################################################################################################
CStr UnformatSectionName(const CStr &strSection)
{
	Assert(!strSection.IsEmpty());

	CStr strUnformattedSection(strSection);
	size_t nPos;

	if((nPos = strUnformattedSection.Find(g_chOpenBracket)) != CStr::not_found)
		strUnformattedSection.Delete(0, nPos + 1);
	if((nPos = strUnformattedSection.ReverseFind(g_chCloseBracket)) != CStr::not_found)
		strUnformattedSection.Delete(nPos, CStr::end);

	strUnformattedSection.Trim();

	return strUnformattedSection;
}


/////////////////////////////////////////////////
// CIniEntry
/////////////////////////////////////////////////

//#################################################################################################
const CStr &CIniEntry::GetEntry(void) const
{
	return *m_itr;
}

//#################################################################################################
CStr CIniEntry::GetKey(void) const
{
	CStr strKey;

	size_t nPos = m_itr->Find(g_chEqual);
	if(nPos != CStr::not_found)
		strKey.Assign(*m_itr, 0, nPos);
	else
		strKey = *m_itr;
	strKey.Trim();

	return strKey;
}

//#################################################################################################
CStr CIniEntry::GetValue(void) const
{
	CStr strValue;

	size_t nPos = m_itr->Find(g_chEqual);
	if(nPos != CStr::not_found)
	{
		strValue.Assign(*m_itr, nPos + 1);
		strValue.Trim();
		strValue.Remove(_N('"'));
	}

	return strValue;
}

//#################################################################################################
void CIniEntry::SetValue(const CStr &strValue)
{
	if(strValue.IsEmpty())
		m_itr->Format(g_szEmptyValue, GetKey());
	else
		m_itr->Format(g_szStrValue, GetKey(), strValue);
}




/////////////////////////////////////////////////
// CIniSection
/////////////////////////////////////////////////

//#################################################################################################
CIniSection::CIniSection(std::list<CStr> &lstParent)
	: m_lstParent(lstParent)
{
}

//#################################################################################################
CStr CIniSection::GetSection(const bool bAddBrackets) const
{
	return (bAddBrackets) ? *m_itr : UnformatSectionName(*m_itr);
}

//#################################################################################################
bool CIniSection::GetValueInt(const CStr &strKey, int &nValue) const
{
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniEntry> pEntry;
	if(GetEntry(strKey, pEntry))
	{
		nValue = StringToInteger<int>(pEntry->GetValue());
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
bool CIniSection::GetValueString(const CStr &strKey, CStr &strValue) const
{
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniEntry> pEntry;
	if(GetEntry(strKey, pEntry))
	{
		strValue = pEntry->GetValue();
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
void CIniSection::SetValueInt(const CStr &strKey, const int nValue)
{
	Assert(!strKey.IsEmpty());

	CIniEntry entry;
	auto itr = m_itr;
	CStr str;

	while(true)
	{
		entry.m_itr = ++itr;
		if(itr == m_lstParent.end() || GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
		{
			str.Format(g_szIntValue, strKey, nValue);
			m_lstParent.insert(itr, str);
			break;
		}
		else if(entry.GetKey().Compare(strKey, true) == 0)
		{
			entry.SetValue(IntegerToString(nValue));
			break;
		}
	}
}

//#################################################################################################
void CIniSection::SetValueString(const CStr &strKey, const CStr &strValue)
{
	Assert(!strKey.IsEmpty());

	CIniEntry entry;
	auto itr = m_itr;
	CStr str;

	while(true)
	{
		entry.m_itr = ++itr;

		if(itr == m_lstParent.end() || GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
		{
			if(strValue.IsEmpty())
				str.Format(g_szEmptyValue, strKey);
			else
				str.Format(g_szStrValue, strKey, strValue);
			m_lstParent.insert(itr, str);
			break;
		}
		else if(entry.GetKey().Compare(strKey, true) == 0)
		{
			entry.SetValue(strValue);
			break;
		}
	}
}

//#################################################################################################
bool CIniSection::DeleteEntry(const CStr &strKey)
{
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniEntry> pEntry;
	if(GetEntry(strKey, pEntry))
	{
		m_lstParent.erase(pEntry->m_itr);
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
void CIniSection::SetEntry(const CStr &strEntry)
{
	Assert(!strEntry.IsEmpty());

	auto itr = m_itr;
	while(true)
	{
		if(++itr == m_lstParent.end() || GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
		{
			m_lstParent.insert(itr, strEntry);
			break;
		}
	}
}

//#################################################################################################
bool CIniSection::GetFirstEntry(std::unique_ptr<CIniEntry> &pEntry) const
{
	bool bSuccess = false;

	if(!pEntry)
		pEntry = std::make_unique<CIniEntry>();
	auto itr = m_itr;

	while(++itr != m_lstParent.end() && GetFirstNonWhiteSpace(*itr) != g_chOpenBracket)
	{
		if(GetFirstNonWhiteSpace(*itr) != g_chSemiColon)
		{
			pEntry->m_itr = itr;
			bSuccess = true;
			break;
		}
	}

	return bSuccess;
}

//#################################################################################################
bool CIniSection::GetNextEntry(std::unique_ptr<CIniEntry> &pEntry) const
{
	bool bSuccess = false;

	std::list<CStr>::iterator itr;

	if(!pEntry)
	{
		pEntry = std::make_unique<CIniEntry>();
		itr = m_itr;
	}
	else
		itr = pEntry->m_itr;

	while(++itr != m_lstParent.end() && GetFirstNonWhiteSpace(*itr) != g_chOpenBracket)
	{
		if(GetFirstNonWhiteSpace(*itr) != g_chSemiColon)
		{
			pEntry->m_itr = itr;
			bSuccess = true;
			break;
		}
	}

	return bSuccess;
}

//#################################################################################################
bool CIniSection::GetEntry(const CStr &strKey, std::unique_ptr<CIniEntry> &pEntry) const
{
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	if(!pEntry)
		pEntry = std::make_unique<CIniEntry>();

	auto itr = m_itr;
	while(++itr != m_lstParent.end() && GetFirstNonWhiteSpace(*itr) != g_chOpenBracket)
	{
		if(GetFirstNonWhiteSpace(*itr) != g_chSemiColon)
		{
			pEntry->m_itr = itr;
			if(pEntry->GetKey().Compare(strKey, true) == 0)
			{
				bSuccess = true;
				break;
			}
		}
	}

	return bSuccess;
}

//#################################################################################################
size_t CIniSection::GetEntryCount(void) const
{
	size_t nCount = 0;

	auto itr = m_itr;
	while(++itr != m_lstParent.end() && GetFirstNonWhiteSpace(*itr) != g_chOpenBracket)
	{
		if(GetFirstNonWhiteSpace(*itr) != g_chSemiColon)
			++nCount;
	}

	return nCount;
}




/////////////////////////////////////////////////
// CIniFile
/////////////////////////////////////////////////

//#################################################################################################
bool CIniFile::Load(PCNSTR szIniPath)
{
	bool bLoaded = false;

	static constexpr auto BUFFER_SIZE = 8192;

	Empty();

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(szIniPath, EFM_ExistingReadOnly, hFile) == FW_NO_ERROR)
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
							m_lstLines.emplace_back(strLine);
							strLine.Empty();
						}
					}

					++szScanFile;
				}

				nErrorCode = FileRead(hFile, pBuf.get(), BUFFER_SIZE, nBytesRead);
				szScanFile = (char16_t*)pBuf.get();
			}

			if(!strLine.IsEmpty())
				m_lstLines.emplace_back(strLine);
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
							m_lstLines.emplace_back(std::move(strLine));
							strLine.Empty();
						}
					}

					++szScanFile;
				}

				nErrorCode = FileRead(hFile, pBuf.get(), BUFFER_SIZE, nBytesRead);
				szScanFile = (PSTR)pBuf.get();
			}

			if(!strLine.IsEmpty())
				m_lstLines.emplace_back(std::move(strLine));
		}

		FileClose(hFile);
		bLoaded = true;
	}

	return bLoaded;
}

//#################################################################################################
bool CIniFile::SaveAs(PCNSTR szIniPath, const EEncodingType eType) const
{
	bool bSaved = false;

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(szIniPath, EFM_CreateWriteOnly, hFile) == FW_NO_ERROR)
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

			CStrW strLine;
			for(const auto &line : m_lstLines)
			{
				strLine = line;
				strLine += g_szEolW;

				// Insert a blank line before each new section
				if(GetFirstNonWhiteSpace(strLine) == g_chOpenBracket)
					FileWrite(hFile, g_szEolW, g_nEolLen * sizeof(wchar_t), nBytesWritten);

				FileWrite(hFile, strLine, strLine.GetSize(), nBytesWritten);
			}
		}
		else
		{	// UTF8 file
			bool bFirstSection = true;

			CStr8 strLine;
			for(const auto &line : m_lstLines)
			{
				strLine = line;
				strLine += g_szEol8;

				// Insert a blank line before each new section
				if(GetFirstNonWhiteSpace(strLine) == g_chOpenBracket)
				{
					if(!bFirstSection)
						FileWrite(hFile, g_szEol8, g_nEolLen, nBytesWritten);
					bFirstSection = false;
				}

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
void CIniFile::ParseFromString(const CStr8 &strIni)
{
	Empty();

	size_t nStart = 0;
	CStr8 strLine = strIni.Tokenize(g_szEol8, nStart);
	while(!strLine.IsEmpty())
	{	// Note, the following move is only successful on MacOS/Linux, on Windows it is still a copy
		m_lstLines.emplace_back(std::move(strLine));
		strLine = strIni.Tokenize(g_szEol8, nStart);
	}
}

//#################################################################################################
void CIniFile::ParseFromString(const CStrW &strIni)
{
	Empty();

	size_t nStart = 0;
	CStrW strLine = strIni.Tokenize(g_szEolW, nStart);
	while(!strLine.IsEmpty())
	{	// Note, the following move is only successful on Windows, on MacOS/Linux it is still a copy
		m_lstLines.emplace_back(std::move(strLine));
		strLine = strIni.Tokenize(g_szEolW, nStart);
	}
}

//#################################################################################################
void CIniFile::SaveToString(CStr8 &strIni) const
{
	strIni.Empty();

	bool bFirstSection = true;
	CStr8 strLine;
	for(const auto &line : m_lstLines)
	{
		strLine = line;
		strLine += g_szEol8;

		// Insert a blank line before each new section
		if(GetFirstNonWhiteSpace(strLine) == g_chOpenBracket)
		{
			if(!bFirstSection)
				strIni += g_szEol8;
			bFirstSection = false;
		}

		strIni += strLine;
	}
}

//#################################################################################################
void CIniFile::SaveToString(CStrW &strIni) const
{
	strIni.Empty();

	bool bFirstSection = true;
	CStrW strLine;
	for(const auto &line : m_lstLines)
	{
		strLine = line;
		strLine += g_szEolW;

		// Insert a blank line before each new section
		if(GetFirstNonWhiteSpace(strLine) == g_chOpenBracket)
		{
			if(!bFirstSection)
				strIni += g_szEolW;
			bFirstSection = false;
		}

		strIni += strLine;
	}
}

//#################################################################################################
void CIniFile::Empty(void)
{
	m_lstLines.clear();
}

//#################################################################################################
bool CIniFile::IsEmpty(void) const noexcept
{
	return m_lstLines.empty();
}

//#################################################################################################
bool CIniFile::GetValueInt(const CStr &strSection, const CStr &strKey, int &nValue)
{
	Assert(!strSection.IsEmpty());
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection))
		bSuccess = pSection->GetValueInt(strKey, nValue);

	return bSuccess;
}

//#################################################################################################
bool CIniFile::GetValueString(const CStr &strSection, const CStr &strKey, CStr &strValue)
{
	Assert(!strSection.IsEmpty());
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection))
		bSuccess = pSection->GetValueString(strKey, strValue);

	return bSuccess;
}

//#################################################################################################
void CIniFile::SetValueInt(const CStr &strSection, const CStr &strKey, const int nValue)
{
	Assert(!strSection.IsEmpty());
	Assert(!strKey.IsEmpty());

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection, true))
		pSection->SetValueInt(strKey, nValue);
}

//#################################################################################################
void CIniFile::SetValueString(const CStr &strSection, const CStr &strKey, const CStr &strValue)
{
	Assert(!strSection.IsEmpty());
	Assert(!strKey.IsEmpty());

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection, true))
		pSection->SetValueString(strKey, strValue);
}

//#################################################################################################
void CIniFile::SetEntry(const CStr &strEntry)
{
	Assert(!strEntry.IsEmpty());

	m_lstLines.insert(m_lstLines.begin(), strEntry);
}

//#################################################################################################
void CIniFile::SetEntry(const CStr &strSection, const CStr &strEntry)
{
	Assert(!strSection.IsEmpty());
	Assert(!strEntry.IsEmpty());

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection, true))
		pSection->SetEntry(strEntry);
}

//#################################################################################################
bool CIniFile::DeleteEntry(const CStr &strSection, const CStr &strKey)
{
	Assert(!strSection.IsEmpty());
	Assert(!strKey.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection))
		bSuccess = pSection->DeleteEntry(strKey);

	return bSuccess;
}

//#################################################################################################
bool CIniFile::GetFirstSection(std::unique_ptr<CIniSection> &pSection)
{
	bool bSuccess = false;

	if(!pSection || pSection->m_lstParent != m_lstLines)
		pSection = std::make_unique<CIniSection>(m_lstLines);

	auto itr = m_lstLines.begin();
	while(itr != m_lstLines.end())
	{
		if(GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
		{
			pSection->m_itr = itr;
			bSuccess = true;
			break;
		}
		++itr;
	}

	return bSuccess;
}

//#################################################################################################
bool CIniFile::GetNextSection(std::unique_ptr<CIniSection> &pSection)
{
	bool bSuccess = false;

	std::list<CStr>::iterator itr;

	if(!pSection || pSection->m_lstParent != m_lstLines)
	{
		pSection = std::make_unique<CIniSection>(m_lstLines);
		itr = m_lstLines.begin();
	}
	else
	{
		itr = pSection->m_itr;
		++itr;
	}

	while(itr != m_lstLines.end())
	{
		if(GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
		{
			pSection->m_itr = itr;
			bSuccess = true;
			break;
		}
		++itr;
	}

	return bSuccess;
}

//#################################################################################################
bool CIniFile::GetSection(const CStr &strSection, std::unique_ptr<CIniSection> &pSection, const bool bCreate)
{
	Assert(!strSection.IsEmpty());

	bool bSuccess = false;

	CStr strUnformattedSection(UnformatSectionName(strSection));

	// pSection was uninitialized or created for a different CIniFile object, create pointing at this CIniFile object
	if(pSection == nullptr || pSection->m_lstParent != m_lstLines)
		pSection = std::make_unique<CIniSection>(m_lstLines);

	for(auto itr = m_lstLines.begin(); itr != m_lstLines.end(); ++itr)
	{
		if(GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
		{
			if(UnformatSectionName(*itr).Compare(strUnformattedSection, true) == 0)
			{
				pSection->m_itr = itr;
				bSuccess = true;
				break;
			}
		}
	}

	if(!bSuccess && bCreate)
	{
		m_lstLines.push_back(FormatSectionName(strUnformattedSection));
		pSection->m_itr = m_lstLines.end();
		--pSection->m_itr;
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
bool CIniFile::DeleteSection(const CStr &strSection)
{
	Assert(!strSection.IsEmpty());

	bool bSuccess = false;

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection))
	{
		do{
			pSection->m_itr = m_lstLines.erase(pSection->m_itr);
		}while(pSection->m_itr != m_lstLines.end() && GetFirstNonWhiteSpace(*pSection->m_itr) != g_chOpenBracket);
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
size_t CIniFile::GetSectionCount(void)
{
	size_t nCount = 0;

	auto itr = m_lstLines.begin();
	while(itr != m_lstLines.end())
	{
		if(GetFirstNonWhiteSpace(*itr) == g_chOpenBracket)
			++nCount;

		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CIniFile::GetEntryCount(const CStr &strSection)
{
	Assert(!strSection.IsEmpty());

	size_t nCount = 0;

	std::unique_ptr<CIniSection> pSection;
	if(GetSection(strSection, pSection))
		nCount = pSection->GetEntryCount();

	return nCount;
}

NS_END
