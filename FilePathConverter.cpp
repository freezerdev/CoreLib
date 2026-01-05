#include "Base.h"
#include "FilePathConverter.h"
#include "FilePathUtils.h"

NS_BEGIN

//#################################################################################################
void CFilePathConverter::Init(void)
{
	Free();

	InitFolders();
	InitDevObjects();
}

//#################################################################################################
void CFilePathConverter::Free(void)
{
	m_vecFolderConvert.clear();
	m_vecDevObjsLookup.clear();
}

//#################################################################################################
bool CFilePathConverter::Resolve(PCNSTR szPath, CStr &strPath) const
{
	bool bResolved;

	CFilePath path(szPath);
	bResolved = Resolve(path);
	strPath = path.Get();

	return bResolved;
}

//#################################################################################################
bool CFilePathConverter::Resolve(CFilePath &path) const
{
	bool bResolved = false;

	CStr strVariable(path.GetFirstSegment().Get());
	if(strVariable.GetFirst() == _N('%') && strVariable.GetLast() == _N('%'))
	{
		strVariable.Trim(_N('%'));

		for(const auto &itr : m_vecFolderConvert)
		{
			if(strVariable.Compare(itr.first, true) == 0)
			{
				path.Delete(0);
				path.Prepend(itr.second);
				bResolved = true;
				break;
			}
		}
	}

	return bResolved;
}

//#################################################################################################
bool CFilePathConverter::DeviceToUser(PCNSTR szDeviceName, CStr &strUserPath) const
{
	strUserPath = szDeviceName;
	return DeviceToUser(strUserPath);
}

//#################################################################################################
bool CFilePathConverter::DeviceToUser(CStr &strPath) const
{
	bool bTranslated = false;

	if(strPath.CompareLeft(_N("\\Device\\Mup\\;LanmanRedirector\\;"), 31) == 0)
	{
		size_t nPos = strPath.Find(_N('\\'), 31);
		if(nPos != CStr::not_found)
			strPath.Delete(12, nPos - 11);
	}

	for(const auto &itr : m_vecDevObjsLookup)
	{
		if(strPath.CompareLeft(itr.first, itr.first.GetLength(), true) == 0)
		{
			strPath.Delete(0, itr.first.GetLength());
			strPath.Prepend(itr.second);
			bTranslated = true;
			break;
		}
	}

	return bTranslated;
}

//#################################################################################################
void CFilePathConverter::InitFolders(void)
{
// Macro to simplify folder entries
#define DECLARE_FOLDER(X) (_N(#X))

	struct KnownFolderTableEntry final
	{
		PCNSTR szVariable;
		const KNOWNFOLDERID &ridFolder;
	}
	static constexpr KnownFolderLookupTable[] =
	{
		{DECLARE_FOLDER(FOLDERID_AccountPictures), FOLDERID_AccountPictures},
		{DECLARE_FOLDER(FOLDERID_AddNewPrograms), FOLDERID_AddNewPrograms},
		{DECLARE_FOLDER(FOLDERID_AdminTools), FOLDERID_AdminTools},
		{DECLARE_FOLDER(FOLDERID_AllAppMods), FOLDERID_AllAppMods},
		{DECLARE_FOLDER(FOLDERID_AppCaptures), FOLDERID_AppCaptures},
		{DECLARE_FOLDER(FOLDERID_AppDataDesktop), FOLDERID_AppDataDesktop},
		{DECLARE_FOLDER(FOLDERID_AppDataDocuments), FOLDERID_AppDataDocuments},
		{DECLARE_FOLDER(FOLDERID_AppDataFavorites), FOLDERID_AppDataFavorites},
		{DECLARE_FOLDER(FOLDERID_AppDataProgramData), FOLDERID_AppDataProgramData},
		{DECLARE_FOLDER(FOLDERID_AppUpdates), FOLDERID_AppUpdates},
		{DECLARE_FOLDER(FOLDERID_ApplicationShortcuts), FOLDERID_ApplicationShortcuts},
		{DECLARE_FOLDER(FOLDERID_AppsFolder), FOLDERID_AppsFolder},
		{DECLARE_FOLDER(FOLDERID_CDBurning), FOLDERID_CDBurning},
		{DECLARE_FOLDER(FOLDERID_CameraRoll), FOLDERID_CameraRoll},
		{DECLARE_FOLDER(FOLDERID_CameraRollLibrary), FOLDERID_CameraRollLibrary},
		{DECLARE_FOLDER(FOLDERID_ChangeRemovePrograms), FOLDERID_ChangeRemovePrograms},
		{DECLARE_FOLDER(FOLDERID_CommonAdminTools), FOLDERID_CommonAdminTools},
		{DECLARE_FOLDER(FOLDERID_CommonOEMLinks), FOLDERID_CommonOEMLinks},
		{DECLARE_FOLDER(FOLDERID_CommonPrograms), FOLDERID_CommonPrograms},
		{DECLARE_FOLDER(FOLDERID_CommonStartMenu), FOLDERID_CommonStartMenu},
		{DECLARE_FOLDER(FOLDERID_CommonStartMenuPlaces), FOLDERID_CommonStartMenuPlaces},
		{DECLARE_FOLDER(FOLDERID_CommonStartup), FOLDERID_CommonStartup},
		{DECLARE_FOLDER(FOLDERID_CommonTemplates), FOLDERID_CommonTemplates},
		{DECLARE_FOLDER(FOLDERID_ComputerFolder), FOLDERID_ComputerFolder},
		{DECLARE_FOLDER(FOLDERID_ConflictFolder), FOLDERID_ConflictFolder},
		{DECLARE_FOLDER(FOLDERID_ConnectionsFolder), FOLDERID_ConnectionsFolder},
		{DECLARE_FOLDER(FOLDERID_Contacts), FOLDERID_Contacts},
		{DECLARE_FOLDER(FOLDERID_ControlPanelFolder), FOLDERID_ControlPanelFolder},
		{DECLARE_FOLDER(FOLDERID_Cookies), FOLDERID_Cookies},
		{DECLARE_FOLDER(FOLDERID_CurrentAppMods), FOLDERID_CurrentAppMods},
		{DECLARE_FOLDER(FOLDERID_Desktop), FOLDERID_Desktop},
		{DECLARE_FOLDER(FOLDERID_DevelopmentFiles), FOLDERID_DevelopmentFiles},
		{DECLARE_FOLDER(FOLDERID_Device), FOLDERID_Device},
		{DECLARE_FOLDER(FOLDERID_DeviceMetadataStore), FOLDERID_DeviceMetadataStore},
		{DECLARE_FOLDER(FOLDERID_Documents), FOLDERID_Documents},
		{DECLARE_FOLDER(FOLDERID_DocumentsLibrary), FOLDERID_DocumentsLibrary},
		{DECLARE_FOLDER(FOLDERID_Downloads), FOLDERID_Downloads},
		{DECLARE_FOLDER(FOLDERID_Favorites), FOLDERID_Favorites},
		{DECLARE_FOLDER(FOLDERID_Fonts), FOLDERID_Fonts},
		{DECLARE_FOLDER(FOLDERID_GameTasks), FOLDERID_GameTasks},
		{DECLARE_FOLDER(FOLDERID_Games), FOLDERID_Games},
		{DECLARE_FOLDER(FOLDERID_History), FOLDERID_History},
		{DECLARE_FOLDER(FOLDERID_HomeGroup), FOLDERID_HomeGroup},
		{DECLARE_FOLDER(FOLDERID_HomeGroupCurrentUser), FOLDERID_HomeGroupCurrentUser},
		{DECLARE_FOLDER(FOLDERID_ImplicitAppShortcuts), FOLDERID_ImplicitAppShortcuts},
		{DECLARE_FOLDER(FOLDERID_InternetCache), FOLDERID_InternetCache},
		{DECLARE_FOLDER(FOLDERID_InternetFolder), FOLDERID_InternetFolder},
		{DECLARE_FOLDER(FOLDERID_Libraries), FOLDERID_Libraries},
		{DECLARE_FOLDER(FOLDERID_Links), FOLDERID_Links},
		{DECLARE_FOLDER(FOLDERID_LocalAppData), FOLDERID_LocalAppData},
		{DECLARE_FOLDER(FOLDERID_LocalAppDataLow), FOLDERID_LocalAppDataLow},
		{DECLARE_FOLDER(FOLDERID_LocalDocuments), FOLDERID_LocalDocuments},
		{DECLARE_FOLDER(FOLDERID_LocalDownloads), FOLDERID_LocalDownloads},
		{DECLARE_FOLDER(FOLDERID_LocalMusic), FOLDERID_LocalMusic},
		{DECLARE_FOLDER(FOLDERID_LocalPictures), FOLDERID_LocalPictures},
//		{DECLARE_FOLDER(FOLDERID_LocalStorage), FOLDERID_LocalStorage},
		{DECLARE_FOLDER(FOLDERID_LocalVideos), FOLDERID_LocalVideos},
		{DECLARE_FOLDER(FOLDERID_LocalizedResourcesDir), FOLDERID_LocalizedResourcesDir},
		{DECLARE_FOLDER(FOLDERID_Music), FOLDERID_Music},
		{DECLARE_FOLDER(FOLDERID_MusicLibrary), FOLDERID_MusicLibrary},
		{DECLARE_FOLDER(FOLDERID_NetHood), FOLDERID_NetHood},
		{DECLARE_FOLDER(FOLDERID_NetworkFolder), FOLDERID_NetworkFolder},
		{DECLARE_FOLDER(FOLDERID_Objects3D), FOLDERID_Objects3D},
		{DECLARE_FOLDER(FOLDERID_OneDrive), FOLDERID_OneDrive},
		{DECLARE_FOLDER(FOLDERID_OriginalImages), FOLDERID_OriginalImages},
		{DECLARE_FOLDER(FOLDERID_PhotoAlbums), FOLDERID_PhotoAlbums},
		{DECLARE_FOLDER(FOLDERID_Pictures), FOLDERID_Pictures},
		{DECLARE_FOLDER(FOLDERID_PicturesLibrary), FOLDERID_PicturesLibrary},
		{DECLARE_FOLDER(FOLDERID_Playlists), FOLDERID_Playlists},
		{DECLARE_FOLDER(FOLDERID_PrintHood), FOLDERID_PrintHood},
		{DECLARE_FOLDER(FOLDERID_PrintersFolder), FOLDERID_PrintersFolder},
		{DECLARE_FOLDER(FOLDERID_Profile), FOLDERID_Profile},
		{DECLARE_FOLDER(FOLDERID_ProgramData), FOLDERID_ProgramData},
		{DECLARE_FOLDER(FOLDERID_ProgramFiles), FOLDERID_ProgramFiles},
		{DECLARE_FOLDER(FOLDERID_ProgramFilesCommon), FOLDERID_ProgramFilesCommon},
		{DECLARE_FOLDER(FOLDERID_ProgramFilesCommonX64), FOLDERID_ProgramFilesCommonX64},
		{DECLARE_FOLDER(FOLDERID_ProgramFilesCommonX86), FOLDERID_ProgramFilesCommonX86},
		{DECLARE_FOLDER(FOLDERID_ProgramFilesX64), FOLDERID_ProgramFilesX64},
		{DECLARE_FOLDER(FOLDERID_ProgramFilesX86), FOLDERID_ProgramFilesX86},
		{DECLARE_FOLDER(FOLDERID_Programs), FOLDERID_Programs},
		{DECLARE_FOLDER(FOLDERID_Public), FOLDERID_Public},
		{DECLARE_FOLDER(FOLDERID_PublicDesktop), FOLDERID_PublicDesktop},
		{DECLARE_FOLDER(FOLDERID_PublicDocuments), FOLDERID_PublicDocuments},
		{DECLARE_FOLDER(FOLDERID_PublicDownloads), FOLDERID_PublicDownloads},
		{DECLARE_FOLDER(FOLDERID_PublicGameTasks), FOLDERID_PublicGameTasks},
		{DECLARE_FOLDER(FOLDERID_PublicLibraries), FOLDERID_PublicLibraries},
		{DECLARE_FOLDER(FOLDERID_PublicMusic), FOLDERID_PublicMusic},
		{DECLARE_FOLDER(FOLDERID_PublicPictures), FOLDERID_PublicPictures},
		{DECLARE_FOLDER(FOLDERID_PublicRingtones), FOLDERID_PublicRingtones},
		{DECLARE_FOLDER(FOLDERID_PublicUserTiles), FOLDERID_PublicUserTiles},
		{DECLARE_FOLDER(FOLDERID_PublicVideos), FOLDERID_PublicVideos},
		{DECLARE_FOLDER(FOLDERID_QuickLaunch), FOLDERID_QuickLaunch},
		{DECLARE_FOLDER(FOLDERID_Recent), FOLDERID_Recent},
		{DECLARE_FOLDER(FOLDERID_RecordedCalls), FOLDERID_RecordedCalls},
		{DECLARE_FOLDER(FOLDERID_RecordedTVLibrary), FOLDERID_RecordedTVLibrary},
		{DECLARE_FOLDER(FOLDERID_RecycleBinFolder), FOLDERID_RecycleBinFolder},
		{DECLARE_FOLDER(FOLDERID_ResourceDir), FOLDERID_ResourceDir},
		{DECLARE_FOLDER(FOLDERID_RetailDemo), FOLDERID_RetailDemo},
		{DECLARE_FOLDER(FOLDERID_Ringtones), FOLDERID_Ringtones},
		{DECLARE_FOLDER(FOLDERID_RoamedTileImages), FOLDERID_RoamedTileImages},
		{DECLARE_FOLDER(FOLDERID_RoamingAppData), FOLDERID_RoamingAppData},
		{DECLARE_FOLDER(FOLDERID_RoamingTiles), FOLDERID_RoamingTiles},
		{DECLARE_FOLDER(FOLDERID_SEARCH_CSC), FOLDERID_SEARCH_CSC},
		{DECLARE_FOLDER(FOLDERID_SEARCH_MAPI), FOLDERID_SEARCH_MAPI},
		{DECLARE_FOLDER(FOLDERID_SampleMusic), FOLDERID_SampleMusic},
		{DECLARE_FOLDER(FOLDERID_SamplePictures), FOLDERID_SamplePictures},
		{DECLARE_FOLDER(FOLDERID_SamplePlaylists), FOLDERID_SamplePlaylists},
		{DECLARE_FOLDER(FOLDERID_SampleVideos), FOLDERID_SampleVideos},
		{DECLARE_FOLDER(FOLDERID_SavedGames), FOLDERID_SavedGames},
		{DECLARE_FOLDER(FOLDERID_SavedPictures), FOLDERID_SavedPictures},
		{DECLARE_FOLDER(FOLDERID_SavedPicturesLibrary), FOLDERID_SavedPicturesLibrary},
		{DECLARE_FOLDER(FOLDERID_SavedSearches), FOLDERID_SavedSearches},
		{DECLARE_FOLDER(FOLDERID_Screenshots), FOLDERID_Screenshots},
		{DECLARE_FOLDER(FOLDERID_SearchHistory), FOLDERID_SearchHistory},
		{DECLARE_FOLDER(FOLDERID_SearchHome), FOLDERID_SearchHome},
		{DECLARE_FOLDER(FOLDERID_SearchTemplates), FOLDERID_SearchTemplates},
		{DECLARE_FOLDER(FOLDERID_SendTo), FOLDERID_SendTo},
		{DECLARE_FOLDER(FOLDERID_SidebarDefaultParts), FOLDERID_SidebarDefaultParts},
		{DECLARE_FOLDER(FOLDERID_SidebarParts), FOLDERID_SidebarParts},
		{DECLARE_FOLDER(FOLDERID_SkyDrive), FOLDERID_SkyDrive},
		{DECLARE_FOLDER(FOLDERID_SkyDriveCameraRoll), FOLDERID_SkyDriveCameraRoll},
		{DECLARE_FOLDER(FOLDERID_SkyDriveDocuments), FOLDERID_SkyDriveDocuments},
		{DECLARE_FOLDER(FOLDERID_SkyDriveMusic), FOLDERID_SkyDriveMusic},
		{DECLARE_FOLDER(FOLDERID_SkyDrivePictures), FOLDERID_SkyDrivePictures},
		{DECLARE_FOLDER(FOLDERID_StartMenu), FOLDERID_StartMenu},
		{DECLARE_FOLDER(FOLDERID_StartMenuAllPrograms), FOLDERID_StartMenuAllPrograms},
		{DECLARE_FOLDER(FOLDERID_Startup), FOLDERID_Startup},
		{DECLARE_FOLDER(FOLDERID_SyncManagerFolder), FOLDERID_SyncManagerFolder},
		{DECLARE_FOLDER(FOLDERID_SyncResultsFolder), FOLDERID_SyncResultsFolder},
		{DECLARE_FOLDER(FOLDERID_SyncSetupFolder), FOLDERID_SyncSetupFolder},
		{DECLARE_FOLDER(FOLDERID_System), FOLDERID_System},
		{DECLARE_FOLDER(FOLDERID_SystemX86), FOLDERID_SystemX86},
		{DECLARE_FOLDER(FOLDERID_Templates), FOLDERID_Templates},
		{DECLARE_FOLDER(FOLDERID_UserPinned), FOLDERID_UserPinned},
		{DECLARE_FOLDER(FOLDERID_UserProfiles), FOLDERID_UserProfiles},
		{DECLARE_FOLDER(FOLDERID_UserProgramFiles), FOLDERID_UserProgramFiles},
		{DECLARE_FOLDER(FOLDERID_UserProgramFilesCommon), FOLDERID_UserProgramFilesCommon},
		{DECLARE_FOLDER(FOLDERID_UsersFiles), FOLDERID_UsersFiles},
		{DECLARE_FOLDER(FOLDERID_UsersLibraries), FOLDERID_UsersLibraries},
		{DECLARE_FOLDER(FOLDERID_Videos), FOLDERID_Videos},
		{DECLARE_FOLDER(FOLDERID_VideosLibrary), FOLDERID_VideosLibrary},
		{DECLARE_FOLDER(FOLDERID_Windows), FOLDERID_Windows}
	};

	for(const auto &itr : KnownFolderLookupTable)
	{
		PWSTR szPath;
		if(SUCCEEDED(SHGetKnownFolderPath(itr.ridFolder, KF_FLAG_DEFAULT, nullptr, &szPath)))
		{
			Assert(StringGetLength(szPath));
			m_vecFolderConvert.emplace_back(CStr(itr.szVariable), CFilePath(szPath));
			CoTaskMemFree(szPath);
		}
	}

	static constexpr PCNSTR EnvVarLookupTable[] =
	{
		_N("AllUsersProfile"),
		_N("AppData"),
		_N("CommonProgramFiles"),
		_N("CommonProgramFiles(x86)"),
		_N("LocalAppData"),
		_N("ProgramData"),
		_N("ProgramFiles"),
		_N("ProgramFiles(x86)"),
		_N("Public"),
		_N("SystemDrive"),
		_N("SystemRoot"),
		_N("Temp"),
		_N("Tmp"),
		_N("UserProfile"),
		_N("WinDir")
	};

	CStr str;
	for(const auto &itr : EnvVarLookupTable)
	{
		str = GetEnvVariable(itr);
		if(!str.IsEmpty())
		{
			CFilePath path(str);
			if(!path.IsEmpty())
				m_vecFolderConvert.emplace_back(CStr(itr), std::move(path));
		}
	}
}

//#################################################################################################
void CFilePathConverter::InitDevObjects(void)
{
	m_vecDevObjsLookup.reserve(6);
	m_vecDevObjsLookup.emplace_back(_N("\\??\\"), CStr());
	m_vecDevObjsLookup.emplace_back(_N("\\\\?\\UNC\\"), _N("\\\\"));
	m_vecDevObjsLookup.emplace_back(_N("\\\\?\\"), CStr());
	m_vecDevObjsLookup.emplace_back(_N("\\Device\\Mup\\"), _N("\\\\"));
	m_vecDevObjsLookup.emplace_back(_N("\\SystemRoot"), GetWindowsFolder().Get());

	NCHAR szDrive[3] = {_N('A'), _N(':'), _N('\0')};
	DWORD dwDrives = GetLogicalDrives();
	wchar_t szDevice[MAX_PATH];
	CStr strDrive;
	for(NCHAR n = 0; n < 26 && dwDrives; ++n)
	{
		if(HAS_FLAG(dwDrives, 0x1))
		{
			szDrive[0] = _N('A') + n;
			strDrive = szDrive;

			if(QueryDosDeviceW(strDrive, szDevice, COUNTOF(szDevice)))
				m_vecDevObjsLookup.emplace_back(szDevice, std::move(strDrive));
		}

		dwDrives >>= 1;
	}
}

NS_END
