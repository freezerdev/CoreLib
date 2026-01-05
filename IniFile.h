#pragma once

#include <list>

NS_BEGIN

// Forward declarations
class CIniEntry;
class CIniSection;
class CIniFile;

//#################################################################################################
class CIniEntry final
{
private:
	friend class CIniSection;

public:
	CIniEntry(void) = default;

	const CStr &GetEntry(void) const;

	CStr GetKey(void) const;
	CStr GetValue(void) const;
	void SetValue(const CStr &strValue);

private:
	std::list<CStr>::iterator m_itr;
};


//#################################################################################################
class CIniSection final
{
private:
	friend class CIniFile;

public:
	explicit CIniSection(std::list<CStr> &lstParent);

	// Get the section name
	CStr GetSection(const bool bAddBrackets = true) const;

	bool GetValueInt(const CStr &strKey, int &nValue) const;
	bool GetValueString(const CStr &strKey, CStr &strValue) const;
	void SetValueInt(const CStr &strKey, const int nValue);
	void SetValueString(const CStr &strKey, const CStr &strValue);

	void SetEntry(const CStr &strEntry);	// Adds an unformatted entry
	bool DeleteEntry(const CStr &strKey);

	bool GetFirstEntry(std::unique_ptr<CIniEntry> &pEntry) const;
	bool GetNextEntry(std::unique_ptr<CIniEntry> &pEntry) const;
	bool GetEntry(const CStr &strKey, std::unique_ptr<CIniEntry> &pEntry) const;

	size_t GetEntryCount(void) const;

private:
	std::list<CStr> &m_lstParent;
	std::list<CStr>::iterator m_itr;
};


//#################################################################################################
class CIniFile final
{
public:
	enum EEncodingType : uint8_t
	{
		EET_Native = 0,
		EET_Utf8,
		EET_Utf16
	};

	CIniFile(void) = default;

	bool Load(PCNSTR szIniPath);
	bool SaveAs(PCNSTR szIniPath, const EEncodingType eType = EET_Native) const;

	void ParseFromString(const CStr8 &strIni);
	void ParseFromString(const CStrW &strIni);
	void SaveToString(CStr8 &strIni) const;
	void SaveToString(CStrW &strIni) const;

	void Empty(void);
	bool IsEmpty(void) const noexcept;

	bool GetValueInt(const CStr &strSection, const CStr &strKey, int &nValue);
	bool GetValueString(const CStr &strSection, const CStr &strKey, CStr &strValue);
	void SetValueInt(const CStr &strSection, const CStr &strKey, const int nValue);
	void SetValueString(const CStr &strSection, const CStr &strKey, const CStr &strValue);

	void SetEntry(const CStr &strEntry);							// Adds an unformatted entry
	void SetEntry(const CStr &strSection, const CStr &strEntry);	// Adds an unformatted entry
	bool DeleteEntry(const CStr &strSection, const CStr &strKey);

	bool GetFirstSection(std::unique_ptr<CIniSection> &pSection);
	bool GetNextSection(std::unique_ptr<CIniSection> &pSection);
	bool GetSection(const CStr &strSection, std::unique_ptr<CIniSection> &pSection, const bool bCreate = false);
	bool DeleteSection(const CStr &strSection);

	size_t GetSectionCount(void);
	size_t GetEntryCount(const CStr &strSection);

private:
	std::list<CStr> m_lstLines;
};

NS_END
