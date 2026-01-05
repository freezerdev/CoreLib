#include "Base.h"
#include "RegKey.h"
#include "DoubleNullStr.h"

NS_BEGIN

/////////////////////////////////////////////////
// CRegBuf
/////////////////////////////////////////////////

//#################################################################################################
CRegBuf::CRegBuf(void)
	: m_pBuf(nullptr),
	m_nBufSize(0),
	m_nDataSize(0),
	m_dwDataType(REG_NONE)
{
}

//#################################################################################################
CRegBuf::CRegBuf(const CRegBuf &src)
	: m_pBuf(nullptr),
	m_nBufSize(0),
	m_nDataSize(0),
	m_dwDataType(REG_NONE)
{
	if(src.m_nBufSize)
	{
		m_pBuf = (PBYTE)std::malloc(src.m_nBufSize);
		if(m_pBuf)
		{
			std::memcpy(m_pBuf, src.m_pBuf, src.m_nBufSize);
			m_nBufSize = src.m_nBufSize;
			m_nDataSize = src.m_nDataSize;
			m_dwDataType = src.m_dwDataType;
		}
	}
}

//#################################################################################################
CRegBuf::CRegBuf(CRegBuf &&src) noexcept
	: m_pBuf(src.m_pBuf),
	m_nBufSize(src.m_nBufSize),
	m_nDataSize(src.m_nDataSize),
	m_dwDataType(src.m_dwDataType)
{
	src.m_pBuf = nullptr;
	src.m_nBufSize = 0;
	src.m_nDataSize = 0;
	src.m_dwDataType = REG_NONE;
}

//#################################################################################################
CRegBuf::CRegBuf(const size_t nBufSize)
	: m_pBuf(nullptr),
	m_nBufSize(0),
	m_nDataSize(0),
	m_dwDataType(REG_NONE)
{
	Alloc(nBufSize);
}

//#################################################################################################
CRegBuf::~CRegBuf(void)
{
	Free();
}

//#################################################################################################
CRegBuf &CRegBuf::operator=(const CRegBuf &src)
{
	if(this != &src && src.m_nBufSize)
	{
		m_pBuf = (PBYTE)std::malloc(src.m_nBufSize);
		if(m_pBuf)
		{
			std::memcpy(m_pBuf, src.m_pBuf, src.m_nBufSize);
			m_nBufSize = src.m_nBufSize;
			m_nDataSize = src.m_nDataSize;
			m_dwDataType = src.m_dwDataType;
		}
	}

	return *this;
}

//#################################################################################################
CRegBuf &CRegBuf::operator=(CRegBuf &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufSize)
			std::free(m_pBuf);

		m_pBuf = src.m_pBuf;
		m_nBufSize = src.m_nBufSize;
		m_nDataSize = src.m_nDataSize;
		m_dwDataType = src.m_dwDataType;

		src.m_pBuf = nullptr;
		src.m_nBufSize = 0;
		src.m_nDataSize = 0;
		src.m_dwDataType = 0;
	}

	return *this;
}

//#################################################################################################
bool CRegBuf::Alloc(const size_t nBufSize)
{
	if(nBufSize > m_nBufSize)
	{	// The buffer is not large enough, realloc it
		PBYTE pBuf;
		if(m_nBufSize)
			pBuf = (PBYTE)std::realloc(m_pBuf, nBufSize);
		else
			pBuf = (PBYTE)std::malloc(nBufSize);

		if(pBuf)
		{
			m_pBuf = pBuf;
			m_nBufSize = nBufSize;
			m_nDataSize = 0;
			m_dwDataType = REG_NONE;
		}
	}

	return (m_pBuf != nullptr);
}

//#################################################################################################
void CRegBuf::Free(void)
{
	std::free(m_pBuf);
	m_pBuf = nullptr;
	m_nBufSize = m_nDataSize = 0;
	m_dwDataType = REG_NONE;
}

//#################################################################################################
void CRegBuf::Empty(void)
{
	if(m_pBuf)
		SecureZeroMemory(m_pBuf, m_nBufSize);
	m_nDataSize = 0;
	m_dwDataType = REG_NONE;
}

//#################################################################################################
bool CRegBuf::IsEmpty(void) const noexcept
{
	return (m_nDataSize == 0);
}

//#################################################################################################
CRegBuf::operator PWSTR(void) const noexcept
{
	return (PWSTR)m_pBuf;
}

//#################################################################################################
CRegBuf::operator PBYTE(void) const noexcept
{
	return m_pBuf;
}

//#################################################################################################
PBYTE CRegBuf::GetBuffer(void) const noexcept
{
	return m_pBuf;
}

//#################################################################################################
size_t CRegBuf::GetBufSize(void) const noexcept
{
	return m_nBufSize;
}

//#################################################################################################
size_t CRegBuf::GetDataSize(void) const noexcept
{
	return m_nDataSize;
}

//#################################################################################################
DWORD CRegBuf::GetDataType(void) const noexcept
{
	return m_dwDataType;
}




/////////////////////////////////////////////////
// CRegKey
/////////////////////////////////////////////////

//#################################################################################################
CRegKey::CRegKey(void)
	: m_hKey(nullptr)
{
}

//#################################################################################################
CRegKey::CRegKey(const CRegKey &src)
	: m_hKey(nullptr)
{
	Assign(src);
}

//#################################################################################################
CRegKey::CRegKey(CRegKey &&src) noexcept
	: m_hKey(src.m_hKey)
{
	src.m_hKey = nullptr;
}

//#################################################################################################
CRegKey::~CRegKey(void)
{
	Close();
}

//#################################################################################################
ERRCODE CRegKey::Open(const CRegPath &path, const REGSAM sam)
{
	Close();

	return ConvertFromNativeErrorCode(RegOpenKeyExW(path.GetHive().GetHive(), path.GetRelativePath().Get(), 0, sam, &m_hKey));
}

//#################################################################################################
ERRCODE CRegKey::Open(const HKEY hKey, PCWSTR szKey, const REGSAM sam)
{
	Close();

	return ConvertFromNativeErrorCode(RegOpenKeyExW(hKey, szKey, 0, sam, &m_hKey));
}

//#################################################################################################
ERRCODE CRegKey::Open(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam)
{
	Assert(regkey.IsOpen());

	Close();

	return ConvertFromNativeErrorCode(RegOpenKeyExW(regkey.m_hKey, szKey, 0, sam, &m_hKey));
}

//#################################################################################################
ERRCODE CRegKey::Open(PCWSTR szKey, const REGSAM sam, CRegKey &regkey)
{
	Assert(IsOpen());

	regkey.Close();

	return ConvertFromNativeErrorCode(RegOpenKeyExW(m_hKey, szKey, 0, sam, &regkey.m_hKey));
}

//#################################################################################################
ERRCODE CRegKey::Open32(const CRegPath &path, const REGSAM sam)
{
	return Open(path, sam | KEY_WOW64_32KEY);
}

//#################################################################################################
ERRCODE CRegKey::Open32(const HKEY hKey, PCWSTR szKey, const REGSAM sam)
{
	return Open(hKey, szKey, sam | KEY_WOW64_32KEY);
}

//#################################################################################################
ERRCODE CRegKey::Open32(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam)
{
	return Open(regkey, szKey, sam | KEY_WOW64_32KEY);
}

//#################################################################################################
ERRCODE CRegKey::Open32(PCWSTR szKey, const REGSAM sam, CRegKey &regkey)
{
	return Open(szKey, sam | KEY_WOW64_32KEY, regkey);
}

//#################################################################################################
ERRCODE CRegKey::Open64(const CRegPath &path, const REGSAM sam)
{
	return Open(path, sam | KEY_WOW64_64KEY);
}

//#################################################################################################
ERRCODE CRegKey::Open64(const HKEY hKey, PCWSTR szKey, const REGSAM sam)
{
	return Open(hKey, szKey, sam | KEY_WOW64_64KEY);
}

//#################################################################################################
ERRCODE CRegKey::Open64(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam)
{
	return Open(regkey, szKey, sam | KEY_WOW64_64KEY);
}

//#################################################################################################
ERRCODE CRegKey::Open64(PCWSTR szKey, const REGSAM sam, CRegKey &regkey)
{
	return Open(szKey, sam | KEY_WOW64_64KEY, regkey);
}

//#################################################################################################
ERRCODE CRegKey::Create(const CRegPath &path, const REGSAM sam, PDWORD pdwDisposition)
{
	Close();

	return ConvertFromNativeErrorCode(RegCreateKeyExW(path.GetHive().GetHive(), path.GetRelativePath().Get(), 0, nullptr, REG_OPTION_NON_VOLATILE, sam, nullptr, &m_hKey, pdwDisposition));
}

//#################################################################################################
ERRCODE CRegKey::Create(const HKEY hKey, PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition)
{
	Close();

	return ConvertFromNativeErrorCode(RegCreateKeyExW(hKey, szKey, 0, nullptr, REG_OPTION_NON_VOLATILE, sam, nullptr, &m_hKey, pdwDisposition));
}

//#################################################################################################
ERRCODE CRegKey::Create(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition)
{
	Assert(regkey.IsOpen());

	Close();

	return ConvertFromNativeErrorCode(RegCreateKeyExW(regkey.m_hKey, szKey, 0, nullptr, REG_OPTION_NON_VOLATILE, sam, nullptr, &m_hKey, pdwDisposition));
}

//#################################################################################################
ERRCODE CRegKey::Create(PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition, CRegKey &regkey)
{
	Assert(IsOpen());

	regkey.Close();

	return ConvertFromNativeErrorCode(RegCreateKeyExW(m_hKey, szKey, 0, nullptr, REG_OPTION_NON_VOLATILE, sam, nullptr, &regkey.m_hKey, pdwDisposition));
}

//#################################################################################################
ERRCODE CRegKey::Create32(const CRegPath &path, const REGSAM sam, PDWORD pdwDisposition)
{
	return Create(path, sam | KEY_WOW64_32KEY, pdwDisposition);
}

//#################################################################################################
ERRCODE CRegKey::Create32(const HKEY hKey, PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition)
{
	return Create(hKey, szKey, sam | KEY_WOW64_32KEY, pdwDisposition);
}

//#################################################################################################
ERRCODE CRegKey::Create32(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition)
{
	return Create(regkey, szKey, sam | KEY_WOW64_32KEY, pdwDisposition);
}

//#################################################################################################
ERRCODE CRegKey::Create32(PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition, CRegKey &regkey)
{
	return Create(szKey, sam | KEY_WOW64_32KEY, pdwDisposition, regkey);
}

//#################################################################################################
ERRCODE CRegKey::Create64(const CRegPath &path, const REGSAM sam, PDWORD pdwDisposition)
{
	return Create(path, sam | KEY_WOW64_64KEY, pdwDisposition);
}

//#################################################################################################
ERRCODE CRegKey::Create64(const HKEY hKey, PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition)
{
	return Create(hKey, szKey, sam | KEY_WOW64_64KEY, pdwDisposition);
}

//#################################################################################################
ERRCODE CRegKey::Create64(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition)
{
	return Create(regkey, szKey, sam | KEY_WOW64_64KEY, pdwDisposition);
}

//#################################################################################################
ERRCODE CRegKey::Create64(PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition, CRegKey &regkey)
{
	return Create(szKey, sam | KEY_WOW64_64KEY, pdwDisposition, regkey);
}

//#################################################################################################
ERRCODE CRegKey::DeleteKey(PCWSTR szKey) const
{
	Assert(szKey);
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegDeleteKeyW(m_hKey, szKey));
}

//#################################################################################################
ERRCODE CRegKey::DeleteKey32(PCWSTR szKey) const
{
	Assert(szKey);
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegDeleteKeyExW(m_hKey, szKey, KEY_WOW64_32KEY, 0));
}

//#################################################################################################
ERRCODE CRegKey::DeleteKey64(PCWSTR szKey) const
{
	Assert(szKey);
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegDeleteKeyExW(m_hKey, szKey, KEY_WOW64_64KEY, 0));
}

//#################################################################################################
bool CRegKey::CreateTree(const CRegPath &path)
{
	if(path.GetSegmentCount() <= 1)
		return false;

	CRegPath pathTemp(path.GetHive());
	size_t nCurrentSegment = 1;
	size_t nTotalSegments = path.GetSegmentCount();

	if(nCurrentSegment == nTotalSegments)
	{	// Path contains one segment, the registry hive
		CRegKey key;
		return (key.Open(pathTemp) == FW_NO_ERROR);
	}
	else
	{
		CRegKey key;
		CRegKey keyNew;
		if(key.Open(pathTemp) == FW_NO_ERROR)
		{
			while(nCurrentSegment < nTotalSegments)
			{
				ERRCODE nErrorCode = key.Open(path.GetSegment(nCurrentSegment), KEY_READ | KEY_WRITE, keyNew);
				if(nErrorCode == FW_ERROR_NOT_FOUND)
				{
					if(key.Create(path.GetSegment(nCurrentSegment), KEY_READ | KEY_WRITE, nullptr, keyNew) != FW_NO_ERROR)
						return false;
				}
				else if(nErrorCode != FW_NO_ERROR)
					return false;

				key.Attach(keyNew.Detach());
				++nCurrentSegment;
			}

			key.Close();
		}
	}

	return true;
}

//#################################################################################################
bool CRegKey::CreateTree32(const CRegPath &path)
{
	if(path.GetSegmentCount() <= 1)
		return false;

	CRegPath pathTemp(path.GetHive());
	size_t nCurrentSegment = 1;
	size_t nTotalSegments = path.GetSegmentCount();

	if(nCurrentSegment == nTotalSegments)
	{	// Path contains one segment, the registry hive
		CRegKey key;
		return (key.Open32(pathTemp) == FW_NO_ERROR);
	}
	else
	{
		CRegKey key;
		CRegKey keyNew;
		if(key.Open32(pathTemp) == FW_NO_ERROR)
		{
			while(nCurrentSegment < nTotalSegments)
			{
				ERRCODE nErrorCode = key.Open32(path.GetSegment(nCurrentSegment), KEY_READ | KEY_WRITE, keyNew);
				if(nErrorCode == FW_ERROR_NOT_FOUND)
				{
					if(key.Create32(path.GetSegment(nCurrentSegment), KEY_READ | KEY_WRITE, nullptr, keyNew) != FW_NO_ERROR)
						return false;
				}
				else if(nErrorCode != FW_NO_ERROR)
					return false;

				key.Attach(keyNew.Detach());
				++nCurrentSegment;
			}

			key.Close();
		}
	}

	return true;
}

//#################################################################################################
bool CRegKey::CreateTree64(const CRegPath &path)
{
	if(path.GetSegmentCount() <= 1)
		return false;

	CRegPath pathTemp(path.GetHive());
	size_t nCurrentSegment = 1;
	size_t nTotalSegments = path.GetSegmentCount();

	if(nCurrentSegment == nTotalSegments)
	{	// Path contains one segment, the registry hive
		CRegKey key;
		return (key.Open64(pathTemp) == FW_NO_ERROR);
	}
	else
	{
		CRegKey key;
		CRegKey keyNew;
		if(key.Open64(pathTemp) == FW_NO_ERROR)
		{
			while(nCurrentSegment < nTotalSegments)
			{
				ERRCODE nErrorCode = key.Open64(path.GetSegment(nCurrentSegment), KEY_READ | KEY_WRITE, keyNew);
				if(nErrorCode == FW_ERROR_NOT_FOUND)
				{
					if(key.Create64(path.GetSegment(nCurrentSegment), KEY_READ | KEY_WRITE, nullptr, keyNew) != FW_NO_ERROR)
						return false;
				}
				else if(nErrorCode != FW_NO_ERROR)
					return false;

				key.Attach(keyNew.Detach());
				++nCurrentSegment;
			}

			key.Close();
		}
	}

	return true;
}

//#################################################################################################
bool CRegKey::DeleteTree(const CRegPath &path)
{
	bool bResult = false;

	if(path.GetSegmentCount() > 1)
	{
		CRegKey key;
		if(key.Open(path.GetLeft(path.GetSegmentCount() - 1), KEY_READ | DELETE) == FW_NO_ERROR)
		{
			if(RegDeleteTreeW(key, path.GetLastSegment()) == ERROR_SUCCESS)
				bResult = true;

			key.Close();
		}
	}

	return bResult;
}

//#################################################################################################
bool CRegKey::DeleteTree32(const CRegPath &path)
{
	bool bResult = false;

	if(path.GetSegmentCount() > 1)
	{
		CRegKey key;
		if(key.Open32(path.GetLeft(path.GetSegmentCount() - 1), KEY_READ | DELETE) == FW_NO_ERROR)
		{
			if(RegDeleteTreeW(key, path.GetLastSegment()) == ERROR_SUCCESS)
				bResult = true;

			key.Close();
		}
	}

	return bResult;
}

//#################################################################################################
bool CRegKey::DeleteTree64(const CRegPath &path)
{
	bool bResult = false;

	if(path.GetSegmentCount() > 1)
	{
		CRegKey key;
		if(key.Open64(path.GetLeft(path.GetSegmentCount() - 1), KEY_READ | DELETE) == FW_NO_ERROR)
		{
			if(RegDeleteTreeW(key, path.GetLastSegment()) == ERROR_SUCCESS)
				bResult = true;

			key.Close();
		}
	}

	return bResult;
}

//#################################################################################################
CRegKey &CRegKey::Attach(HKEY hKey)
{
	Close();

	m_hKey = hKey;

	return *this;
}

//#################################################################################################
HKEY CRegKey::Detach(void)
{	// Release the key, it is up to the caller to close the handle
	HKEY hKey = m_hKey;
	m_hKey = nullptr;

	return hKey;
}

//#################################################################################################
CRegKey &CRegKey::operator=(const CRegKey &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CRegKey &CRegKey::operator=(CRegKey &&src) noexcept
{
	if(this != &src)
	{
		if(m_hKey)
			RegCloseKey(m_hKey);

		m_hKey = src.m_hKey;

		src.m_hKey = nullptr;
	}

	return *this;
}

//#################################################################################################
void CRegKey::Assign(const CRegKey &regkey)
{
	if(this != &regkey)
	{
		Close();

		if(regkey.m_hKey)
			DuplicateHandle(GetCurrentProcess(), regkey.m_hKey, GetCurrentProcess(), (PHANDLE)&m_hKey, 0, false, DUPLICATE_SAME_ACCESS);
	}
}

//#################################################################################################
void CRegKey::Close(void)
{
	if(m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = nullptr;
	}
}

//#################################################################################################
bool CRegKey::IsOpen(void) const noexcept
{
	return (m_hKey != nullptr);
}

//#################################################################################################
CRegKey::operator HKEY(void) const noexcept
{
	return m_hKey;
}

//#################################################################################################
bool CRegKey::KeyExists(PCWSTR szKey) const
{
	bool bExists = false;

	if(m_hKey)
	{
		HKEY hKey;
		if(RegOpenKeyExW(m_hKey, szKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			bExists = true;
		}
	}

	return bExists;
}

//#################################################################################################
bool CRegKey::ValueExists(PCWSTR szValue) const
{
	bool bExists = false;

	if(m_hKey && RegQueryValueExW(m_hKey, szValue, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
		bExists = true;

	return bExists;
}

//#################################################################################################
size_t CRegKey::GetKeyCount(void) const
{
	DWORD dwKeyCount = 0;

	RegQueryInfoKeyW(m_hKey, nullptr, nullptr, nullptr, &dwKeyCount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	return dwKeyCount;
}

//#################################################################################################
ERRCODE CRegKey::EnumKey(CRegBuf &regbuf, DWORD &dwIndex) const
{
	ERRCODE nErrorCode;

	regbuf.Empty();

	if(dwIndex == 0)
	{
		DWORD dwMaxKeyLen;
		if(RegQueryInfoKeyW(m_hKey, nullptr, nullptr, nullptr, nullptr, &dwMaxKeyLen, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
			regbuf.Alloc(((size_t)dwMaxKeyLen + 1) * sizeof(wchar_t));
	}

	DWORD dwLen = (DWORD)(regbuf.m_nBufSize / sizeof(wchar_t));
	nErrorCode = ConvertFromNativeErrorCode(RegEnumKeyExW(m_hKey, dwIndex++, (PWSTR)regbuf.m_pBuf, &dwLen, nullptr, nullptr, nullptr, nullptr));
	if(nErrorCode == FW_NO_ERROR && dwLen <= regbuf.m_nBufSize / sizeof(wchar_t))
		regbuf.m_nDataSize = dwLen * sizeof(wchar_t);

	return nErrorCode;
}

//#################################################################################################
size_t CRegKey::GetValueCount(void) const
{
	DWORD dwValueCount = 0;

	RegQueryInfoKeyW(m_hKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &dwValueCount, nullptr, nullptr, nullptr, nullptr);

	return dwValueCount;
}

//#################################################################################################
ERRCODE CRegKey::EnumValue(CRegBuf &regbuf, DWORD &dwIndex) const
{
	ERRCODE nErrorCode;

	regbuf.Empty();

	if(dwIndex == 0)
	{
		DWORD dwMaxValueLen;
		if(RegQueryInfoKeyW(m_hKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &dwMaxValueLen, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
			regbuf.Alloc(((size_t)dwMaxValueLen + 1) * sizeof(wchar_t));
	}

	DWORD dwLen = (DWORD)(regbuf.m_nBufSize / sizeof(wchar_t));
	nErrorCode = ConvertFromNativeErrorCode(RegEnumValueW(m_hKey, dwIndex++, (PWSTR)regbuf.m_pBuf, &dwLen, nullptr, &regbuf.m_dwDataType, nullptr, nullptr));
	if(nErrorCode == FW_NO_ERROR && dwLen <= regbuf.m_nBufSize / sizeof(wchar_t))
		regbuf.m_nDataSize = dwLen * sizeof(wchar_t);

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegKey::DeleteValue(PCWSTR szValue) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegDeleteValueW(m_hKey, szValue));
}

//#################################################################################################
ERRCODE CRegKey::GetValue(PCWSTR szValue, CRegBuf &regbuf) const
{
	Assert(m_hKey);

	ERRCODE nErrorCode;

	regbuf.Empty();

	DWORD dwDataSize = (DWORD)regbuf.m_nBufSize;
	nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &regbuf.m_dwDataType, regbuf.m_pBuf, &dwDataSize));
	if(nErrorCode == FW_NO_ERROR && dwDataSize <= regbuf.m_nBufSize)
		regbuf.m_nDataSize = dwDataSize;
	else if(nErrorCode == FW_ERROR_MORE_DATA || dwDataSize > regbuf.m_nBufSize)
	{
		regbuf.Alloc(dwDataSize);
		dwDataSize = (DWORD)regbuf.m_nBufSize;
		nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &regbuf.m_dwDataType, regbuf.m_pBuf, &dwDataSize));
		if(nErrorCode == FW_NO_ERROR && dwDataSize <= regbuf.m_nBufSize)
			regbuf.m_nDataSize = dwDataSize;
	}

	// RegQueryValueEx() does not guarantee null termination for strings,
	// so for string types check for null terminators and add them if missing.
	if(nErrorCode == FW_NO_ERROR && (regbuf.m_dwDataType == REG_SZ || regbuf.m_dwDataType == REG_EXPAND_SZ) && regbuf.m_nDataSize >= 2 && ((PCWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t) - 1] != g_chNullW)
	{
		if(regbuf.m_nBufSize > regbuf.m_nDataSize + sizeof(wchar_t))
		{
			((PWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t)] = g_chNullW;
			regbuf.m_nDataSize += sizeof(wchar_t);
		}
		else
		{
			auto pBufNew = std::make_unique<BYTE[]>(regbuf.m_nDataSize);
			CopyMemory(pBufNew.get(), regbuf.m_pBuf, regbuf.m_nDataSize);
			SecureZeroMemory(pBufNew.get() + regbuf.m_nDataSize, sizeof(wchar_t));
			std::free(regbuf.m_pBuf);
			regbuf.m_pBuf = pBufNew.get();
			regbuf.m_nBufSize += sizeof(wchar_t);
			regbuf.m_nDataSize += sizeof(wchar_t);
		}
	}
	else if(nErrorCode == FW_NO_ERROR && regbuf.m_dwDataType == REG_MULTI_SZ && regbuf.m_nDataSize >= 4 && (((PCWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t) - 2] != g_chNullW || ((PCWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t) - 1] != g_chNullW))
	{
		if(((PCWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t) - 1] == g_chNullW)
		{	// Single null terminated
			if(regbuf.m_nBufSize > regbuf.m_nDataSize + sizeof(wchar_t))
			{
				((PWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t)] = g_chNullW;
				regbuf.m_nDataSize += sizeof(wchar_t);
			}
			else
			{
				auto pBufNew = std::make_unique<BYTE[]>(regbuf.m_nDataSize);
				CopyMemory(pBufNew.get(), regbuf.m_pBuf, regbuf.m_nDataSize);
				SecureZeroMemory(pBufNew.get() + regbuf.m_nDataSize, sizeof(wchar_t));
				std::free(regbuf.m_pBuf);
				regbuf.m_pBuf = pBufNew.get();
				regbuf.m_nBufSize += sizeof(wchar_t);
				regbuf.m_nDataSize += sizeof(wchar_t);
			}
		}
		else
		{	// Missing both null terminators
			if(regbuf.m_nBufSize > regbuf.m_nDataSize + 2 * sizeof(wchar_t))
			{
				((PWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t)] = g_chNullW;
				((PWSTR)regbuf.m_pBuf)[regbuf.m_nDataSize / sizeof(wchar_t) + 1] = g_chNullW;
				regbuf.m_nDataSize += 2 * sizeof(wchar_t);
			}
			else
			{
				auto pBufNew = std::make_unique<BYTE[]>(regbuf.m_nDataSize + 2);
				CopyMemory(pBufNew.get(), regbuf.m_pBuf, regbuf.m_nDataSize);
				SecureZeroMemory(pBufNew.get() + regbuf.m_nDataSize, 2 * sizeof(wchar_t));
				std::free(regbuf.m_pBuf);
				regbuf.m_pBuf = pBufNew.get();
				regbuf.m_nBufSize += 2 * sizeof(wchar_t);
				regbuf.m_nDataSize += 2 * sizeof(wchar_t);
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegKey::GetValue(PCWSTR szValue, CStrW &strData, bool *pbExpandSz) const
{
	Assert(m_hKey);

	ERRCODE nErrorCode;

	strData.Empty();
	if(pbExpandSz)
		*pbExpandSz = false;

	DWORD dwDataType;
	DWORD dwDataSize = 0;
	nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, nullptr, &dwDataSize));
	if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_MORE_DATA)
	{
		if(dwDataType == REG_SZ || dwDataType == REG_EXPAND_SZ)
		{
			DWORD dwStrLen = dwDataSize / sizeof(wchar_t) + 1;	// RegQueryValueEx() does not guarantee null termination
			auto sz = std::make_unique<wchar_t[]>(dwStrLen);
			dwDataSize = dwStrLen * sizeof(wchar_t);
			nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, (PBYTE)sz.get(), &dwDataSize));
			if(nErrorCode == FW_NO_ERROR)
			{
				sz[dwStrLen - 1] = g_chNullW;
				strData = sz.get();
				if(pbExpandSz && dwDataType == REG_EXPAND_SZ)
					*pbExpandSz = true;
			}
		}
		else
			nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegKey::GetValue(PCWSTR szValue, DWORD &nData) const
{
	Assert(m_hKey);

	ERRCODE nErrorCode;

	DWORD dwDataType;
	DWORD dwDataSize = sizeof(nData);
	nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, (PBYTE)&nData, &dwDataSize));
	if(nErrorCode == FW_NO_ERROR && (dwDataType != REG_DWORD || dwDataSize != sizeof(nData)))
	{
		nData = 0;
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegKey::GetValue(PCWSTR szValue, uint64_t &nData) const
{
	Assert(m_hKey);

	ERRCODE nErrorCode;

	DWORD dwDataType;
	DWORD dwDataSize = sizeof(nData);
	nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, (PBYTE)&nData, &dwDataSize));
	if(nErrorCode == FW_NO_ERROR && (dwDataType != REG_QWORD || dwDataSize != sizeof(nData)))
	{
		nData = 0;
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegKey::GetValue(PCWSTR szValue, CDoubleNullStrW &strData) const
{
	Assert(m_hKey);

	ERRCODE nErrorCode;

	strData.Empty();

	DWORD dwDataType;
	DWORD dwDataSize = 0;
	nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, nullptr, &dwDataSize));
	if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_MORE_DATA)
	{
		if(dwDataType == REG_MULTI_SZ)
		{
			DWORD dwStrLen = dwDataSize / sizeof(wchar_t) + 2;	// RegQueryValueEx() does not guarantee null termination
			auto sz = std::make_unique<wchar_t[]>(dwStrLen);
			dwDataSize = dwStrLen * sizeof(wchar_t);
			nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, (PBYTE)sz.get(), &dwDataSize));
			if(nErrorCode == FW_NO_ERROR)
			{
				sz[dwStrLen - 2] = g_chNullW;
				sz[dwStrLen - 1] = g_chNullW;
				strData.Assign(sz.get(), true);
			}
		}
		else if(dwDataType == REG_SZ || dwDataType == REG_EXPAND_SZ)
		{
			DWORD dwStrLen = dwDataSize / sizeof(wchar_t) + 2;	// RegQueryValueEx() does not guarantee null termination
			auto sz = std::make_unique<wchar_t[]>(dwStrLen);
			dwDataSize = dwStrLen * sizeof(wchar_t);
			nErrorCode = ConvertFromNativeErrorCode(RegQueryValueExW(m_hKey, szValue, nullptr, &dwDataType, (PBYTE)sz.get(), &dwDataSize));
			if(nErrorCode == FW_NO_ERROR)
			{
				sz[dwStrLen - 2] = g_chNullW;
				sz[dwStrLen - 1] = g_chNullW;
				strData.Assign(sz.get(), true);
			}
		}
		else
			nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, const DWORD dwType, PCBYTE pData, const size_t nDataSize) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, dwType, pData, (DWORD)nDataSize));
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, const CRegBuf &regbuf) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, regbuf.m_dwDataType, regbuf.m_pBuf, (DWORD)regbuf.m_nDataSize));
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, const CStrW &strData, const bool bExpandSz) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, bExpandSz ? REG_EXPAND_SZ : REG_SZ, (PCBYTE)(PCWSTR)strData, (DWORD)strData.GetSize(true)));
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, PCWSTR szData, const size_t nStrLen, const bool bExpandSz) const
{
	Assert(m_hKey);

	size_t nDataLen = nStrLen;
	if(nDataLen == (size_t)-1)
		nDataLen = StringGetLength(szData);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, bExpandSz ? REG_EXPAND_SZ : REG_SZ, (PCBYTE)szData, (DWORD)((nDataLen + 1) * sizeof(wchar_t))));
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, const DWORD nData) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, REG_DWORD, (PCBYTE)&nData, sizeof(nData)));
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, const uint64_t nData) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, REG_QWORD, (PCBYTE)&nData, sizeof(nData)));
}

//#################################################################################################
ERRCODE CRegKey::SetValue(PCWSTR szValue, const CDoubleNullStrW &strData) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegSetValueExW(m_hKey, szValue, 0, REG_MULTI_SZ, (PCBYTE)(PCWSTR)strData, (DWORD)strData.GetSize()));
}

//#################################################################################################
ERRCODE CRegKey::DisableReflection(void) const
{
#ifdef _M_X64
	return NO_ERROR;
#else
	ERRCODE nErrorCode = FW_ERROR_INVALID_HANDLE;

	if(m_hKey)
		nErrorCode = ConvertFromNativeErrorCode(RegDisableReflectionKey(m_hKey));

	return nErrorCode;
#endif
}

//#################################################################################################
ERRCODE CRegKey::EnableReflection(void) const
{
#ifdef _M_X64
	return NO_ERROR;
#else
	ERRCODE nErrorCode = FW_ERROR_INVALID_HANDLE;

	if(m_hKey)
		nErrorCode = ConvertFromNativeErrorCode(RegEnableReflectionKey(m_hKey));

	return nErrorCode;
#endif
}

//#################################################################################################
bool CRegKey::IsReflected(void) const
{
#ifdef _M_X64
	return false;
#else
	BOOL bReflected = FALSE;

	if(m_hKey)
		RegQueryReflectionKey(m_hKey, &bReflected);

	return (bReflected != FALSE);
#endif
}

//#################################################################################################
ERRCODE CRegKey::Flush(void) const
{
	Assert(m_hKey);

	return ConvertFromNativeErrorCode(RegFlushKey(m_hKey));
}

NS_END
