#pragma once

#include <vector>

NS_BEGIN

//#################################################################################################
class CFilePathConverter final
{
public:
	CFilePathConverter(void) = default;

	void Init(void);
	void Free(void);

	// The following variables are resolved if the path begins with them:
	//  1) Known folders variables like '%FOLDERID_Windows%'
	//  2) Environment variables like '%Temp%'
	bool Resolve(PCNSTR szPath, CStr &strPath) const;
	bool Resolve(CFilePath &path) const;

	// The following device formats are resolved if the path begins with them:
	//  1) '\Device\HarddiskVolumeX' is replaced with appropriate drive letter
	//  2) '\SystemRoot' is replaced with the windows folder
	//  3) Device prefix '\??\' is removed
	//  4) Kernel prefix '\\?\' is removed
	//  5) Kernel UNC prefix '\\?\UNC\' is replaced with '\\'
	bool DeviceToUser(PCNSTR szDeviceName, CStr &strUserPath) const;
	bool DeviceToUser(CStr &strPath) const;

private:
	std::vector<std::pair<CStr, CFilePath>> m_vecFolderConvert;
	std::vector<std::pair<CStr, CStr>> m_vecDevObjsLookup;

	void InitFolders(void);
	void InitDevObjects(void);
};

NS_END
