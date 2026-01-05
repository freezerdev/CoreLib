#pragma once

#include "RegPath.h"

NS_BEGIN

// Forward declarations
class CRegKey;
class CDoubleNullStrW;

//#################################################################################################
class CRegBuf final
{
private:
	friend class CRegKey;

public:
	CRegBuf(void);
	CRegBuf(const CRegBuf &src);
	CRegBuf(CRegBuf &&src) noexcept;
	explicit CRegBuf(const size_t nBufSize);
	~CRegBuf(void);

	CRegBuf &operator=(const CRegBuf &src);
	CRegBuf &operator=(CRegBuf &&src) noexcept;

	bool Alloc(const size_t nBufSize);
	void Free(void);
	void Empty(void);

	bool IsEmpty(void) const noexcept;

	operator PWSTR(void) const noexcept;
	operator PBYTE(void) const noexcept;

	PBYTE GetBuffer(void) const noexcept;
	size_t GetBufSize(void) const noexcept;
	size_t GetDataSize(void) const noexcept;
	DWORD GetDataType(void) const noexcept;

private:
	PBYTE m_pBuf;
	size_t m_nBufSize;
	size_t m_nDataSize;
	DWORD m_dwDataType;
};


//#################################################################################################
class CRegKey final
{
public:
	CRegKey(void);
	CRegKey(const CRegKey &src);
	CRegKey(CRegKey &&src) noexcept;
	~CRegKey(void);

	// Open a registry key from the native hive (64 on 64-bit and 32 on 32-bit)
	ERRCODE Open(const CRegPath &path, const REGSAM sam = KEY_READ);
	ERRCODE Open(const HKEY hKey, PCWSTR szKey, const REGSAM sam = KEY_READ);
	ERRCODE Open(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam = KEY_READ);
	ERRCODE Open(PCWSTR szKey, const REGSAM sam, CRegKey &regkey);

	// Open a registry key from the 32-bit hive
	ERRCODE Open32(const CRegPath &path, const REGSAM sam = KEY_READ);
	ERRCODE Open32(const HKEY hKey, PCWSTR szKey, const REGSAM sam = KEY_READ);
	ERRCODE Open32(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam = KEY_READ);
	ERRCODE Open32(PCWSTR szKey, const REGSAM sam, CRegKey &regkey);

	// Open a registry key from the 64-bit hive
	ERRCODE Open64(const CRegPath &path, const REGSAM sam = KEY_READ);
	ERRCODE Open64(const HKEY hKey, PCWSTR szKey, const REGSAM sam = KEY_READ);
	ERRCODE Open64(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam = KEY_READ);
	ERRCODE Open64(PCWSTR szKey, const REGSAM sam, CRegKey &regkey);

	// Creates a registry key in the native hive (64 on 64-bit and 32 on 32-bit)
	ERRCODE Create(const CRegPath &path, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create(const HKEY hKey, PCWSTR szKey, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create(PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition, CRegKey &regkey);

	// Creates a registry key in the 32-bit hive
	ERRCODE Create32(const CRegPath &path, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create32(const HKEY hKey, PCWSTR szKey, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create32(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create32(PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition, CRegKey &regkey);

	// Creates a registry key in the 64-bit hive
	ERRCODE Create64(const CRegPath &path, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create64(const HKEY hKey, PCWSTR szKey, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create64(const CRegKey &regkey, PCWSTR szKey, const REGSAM sam = KEY_READ | KEY_WRITE, PDWORD pdwDisposition = nullptr);
	ERRCODE Create64(PCWSTR szKey, const REGSAM sam, PDWORD pdwDisposition, CRegKey &regkey);

	ERRCODE DeleteKey(PCWSTR szKey) const;
	ERRCODE DeleteKey32(PCWSTR szKey) const;
	ERRCODE DeleteKey64(PCWSTR szKey) const;

	static bool CreateTree(const CRegPath &path);
	static bool CreateTree32(const CRegPath &path);
	static bool CreateTree64(const CRegPath &path);
	static bool DeleteTree(const CRegPath &path);
	static bool DeleteTree32(const CRegPath &path);
	static bool DeleteTree64(const CRegPath &path);

	CRegKey &Attach(HKEY hKey);
	HKEY Detach(void);

	CRegKey &operator=(const CRegKey &src);
	CRegKey &operator=(CRegKey &&src) noexcept;
	void Assign(const CRegKey &regkey);

	void Close(void);

	bool IsOpen(void) const noexcept;
	operator HKEY(void) const noexcept;

	bool KeyExists(PCWSTR szKey) const;
	bool ValueExists(PCWSTR szValue) const;

	size_t GetKeyCount(void) const;
	ERRCODE EnumKey(CRegBuf &regbuf, DWORD &dwIndex) const;

	size_t GetValueCount(void) const;
	ERRCODE EnumValue(CRegBuf &regbuf, DWORD &dwIndex) const;
	ERRCODE DeleteValue(PCWSTR szValue) const;

	ERRCODE GetValue(PCWSTR szValue, CRegBuf &regbuf) const;
	ERRCODE GetValue(PCWSTR szValue, CStrW &strData, bool *pbExpandSz = nullptr) const;
	ERRCODE GetValue(PCWSTR szValue, DWORD &nData) const;
	ERRCODE GetValue(PCWSTR szValue, uint64_t &nData) const;
	ERRCODE GetValue(PCWSTR szValue, CDoubleNullStrW &strData) const;

	ERRCODE SetValue(PCWSTR szValue, const DWORD dwType, PCBYTE pData, const size_t nDataSize) const;
	ERRCODE SetValue(PCWSTR szValue, const CRegBuf &regbuf) const;
	ERRCODE SetValue(PCWSTR szValue, const CStrW &strData, const bool bExpandSz = false) const;
	ERRCODE SetValue(PCWSTR szValue, PCWSTR szData, const size_t nStrLen = -1, const bool bExpandSz = false) const;
	ERRCODE SetValue(PCWSTR szValue, const DWORD nData) const;
	ERRCODE SetValue(PCWSTR szValue, const uint64_t nData) const;
	ERRCODE SetValue(PCWSTR szValue, const CDoubleNullStrW &strData) const;

	ERRCODE DisableReflection(void) const;
	ERRCODE EnableReflection(void) const;
	bool IsReflected(void) const;

	ERRCODE Flush(void) const;

private:
	HKEY m_hKey;
};

NS_END
