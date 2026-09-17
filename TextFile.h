#pragma once

#include <vector>

NS_BEGIN

//#################################################################################################
class CTextFile final
{
public:
	enum class EEncodingType : uint8_t
	{
		Native,
		Utf8,
		Utf16
	};

	CTextFile(void) = default;

	bool Load(PCNSTR szTxtPath);
	bool SaveAs(PCNSTR szTxtPath, const EEncodingType eType = EEncodingType::Native) const;

	void ParseFromString(const CStr8 &strTxt);
	void ParseFromString(const CStrW &strTxt);
	void SaveToString(CStr8 &strTxt) const;
	void SaveToString(CStrW &strTxt) const;

	void Empty(void);
	bool IsEmpty(void) const noexcept;

	size_t GetLineCount(void) const;

	void AppendLine(PCNSTR sz);
	void AppendBlankLine(void);

	size_t GetFileSize(const EEncodingType eType = EEncodingType::Native) const;

	std::vector<CStr>::const_iterator begin(void) const;
	std::vector<CStr>::const_iterator end(void) const;

	std::vector<CStr>::iterator begin(void);
	std::vector<CStr>::iterator end(void);

private:
	std::vector<CStr> m_vecLines;
};

NS_END
