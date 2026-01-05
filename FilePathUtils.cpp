#include "Base.h"
#include "FilePathUtils.h"
#ifdef _WIN32
#include "FileSystemUtils.h"
#include <lm.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#include <sys/attr.h>
#endif

NS_BEGIN

#ifdef _WIN32
//#################################################################################################
CFilePathW GetKnownFolder(const KNOWNFOLDERID &id)
{
	CFilePathW path;

	PWSTR szPath;
	if(SUCCEEDED(SHGetKnownFolderPath(id, KF_FLAG_DEFAULT, nullptr, &szPath)))
	{
		path = szPath;
		CoTaskMemFree(szPath);
	}

	return path;
}

//#################################################################################################
CFilePathW GetWindowsFolder(void)
{
	CFilePathW path;

	wchar_t szPath[MAX_PATH];
	if(GetWindowsDirectoryW(szPath, MAX_PATH))
		path = szPath;

	return path;
}

//#################################################################################################
CFilePathW GetSystemFolder(void)
{
	CFilePathW path;

	wchar_t szPath[MAX_PATH];
	if(GetSystemDirectoryW(szPath, MAX_PATH))
		path = szPath;

	return path;
}

//#################################################################################################
CFilePathW GetCurrentFolder(const CFilePathSegmentW &drive)
{
	CFilePathW path;

	if(drive.IsDrive())
	{
		DWORD dwLen = GetFullPathNameW(drive.Get(), 0, nullptr, nullptr);
		if(dwLen)
		{
			auto szPath = std::make_unique<wchar_t[]>(dwLen);
			if(GetFullPathNameW(drive.Get(), dwLen, szPath.get(), nullptr))
				path = szPath.get();
		}
	}

	return path;
}
#endif

//#################################################################################################
CFilePath GetTempFolder(void)
{
	CFilePath path;

#ifdef _WIN32
	wchar_t szPath[MAX_PATH];
	if(GetTempPathW(MAX_PATH, szPath))
		path = szPath;
#elif __APPLE__
	CStr8 strTemp = GetEnvVariable("TMPDIR");
	if(strTemp.IsEmpty())
		strTemp = "/private/tmp";

	path = strTemp;
#elif __linux__
	CStr8 strTemp = GetEnvVariable("TEMP");
	if(strTemp.IsEmpty())
	{
		strTemp = GetEnvVariable("TMP");
		if(strTemp.IsEmpty())
			strTemp = "/tmp";
	}

	path = strTemp;
#endif

	return path;
}

//#################################################################################################
CFilePath GetCurrentFolder(void)
{
	CFilePath path;

#ifdef _WIN32
	wchar_t szPath[MAX_PATH];
	if(GetCurrentDirectoryW(MAX_PATH, szPath))
		path = szPath;
#else
	char szPath[MAX_PATH];
	if(getcwd(szPath, MAX_PATH))
		path = szPath;
#endif

	return path;
}

//#################################################################################################
bool SetCurrentFolder(PCNSTR szPath)
{
#ifdef _WIN32
	return (SetCurrentDirectoryW(szPath) != FALSE);
#else
	return (chdir(szPath) != -1);
#endif
}

//#################################################################################################
CFilePath GetExecutablePath(void)
{
	CFilePath path;

#ifdef _WIN32
	wchar_t szPath[MAX_PATH];
	if(GetModuleFileNameW(GetModuleHandleW(nullptr), szPath, MAX_PATH))
		path = szPath;
#elif __APPLE__
	char szPath[PATH_MAX] = {0};
	uint32_t nLen = sizeof(szPath);
	if(_NSGetExecutablePath(szPath, &nLen) == 0)
		path = szPath;
#elif __linux__
	char szPath[MAX_PATH] = {0};
	if(readlink("/proc/self/exe", szPath, MAX_PATH) != -1)
		path = szPath;
#endif

	Canonicalize(path);

	return path;
}

#ifdef _WIN32
//#################################################################################################
bool ConvertToLong(CStrW &str)
{
	bool bConverted = false;

	wchar_t szLong[MAX_PATH];
	CStrW strTemp(str);
	DWORD dwErrorCode;
	size_t nStart = CStrW::end;

	do{
		if(strTemp.IsEmpty())
			break;

		DWORD dwResult = GetLongPathNameW(strTemp, szLong, MAX_PATH);
		if(dwResult > MAX_PATH)
			break;
		else if(dwResult)
		{
			if(str.CompareLeft(szLong, strTemp.GetLength(), true) != 0)
			{
				str.Delete(0, strTemp.GetLength());
				str.Prepend(szLong);
				bConverted = true;
			}
			break;
		}
		else
			dwErrorCode = GetLastError();

		size_t nPos = strTemp.ReverseFind(L'\\', nStart);
		if(nPos == 0 || nPos == CStrW::not_found)
			break;
		else
		{
			strTemp.CropLeft(nPos);
			nStart = nPos;
		}
	}while(dwErrorCode == ERROR_FILE_NOT_FOUND || dwErrorCode == ERROR_PATH_NOT_FOUND);

	return bConverted;
}

//#################################################################################################
bool ConvertToLong(CFilePathW &path)
{
	bool bConverted = false;

	CFilePathW pathTemp(path);

	size_t nSegmentCount = path.GetSegmentCount();
	DWORD dwErrorCode;

	do{
		if(pathTemp.IsEmpty())
			break;

		DWORD dwLen = GetLongPathNameW(pathTemp.Get(), nullptr, 0);
		if(dwLen)
		{
			auto szLong = std::make_unique<wchar_t[]>(dwLen);
			if(GetLongPathNameW(pathTemp.Get(), szLong.get(), dwLen))
			{
				CFilePathW pathNew(szLong.get());
				if(!path.CompareLeft(pathNew, nSegmentCount))
				{
					path.TrimLeft(nSegmentCount);
					path.Prepend(pathNew);
					bConverted = true;
				}
				break;
			}
			else
				dwErrorCode = GetLastError();
		}
		else
			dwErrorCode = GetLastError();

		pathTemp.TrimRight();
		--nSegmentCount;
	}while(dwErrorCode == ERROR_FILE_NOT_FOUND || dwErrorCode == ERROR_PATH_NOT_FOUND);

	return bConverted;
}

//#################################################################################################
bool ConvertToShort(CStrW &str)
{
	bool bConverted = false;

	wchar_t szShort[MAX_PATH];
	CStrW strTemp(str);
	DWORD dwErrorCode;
	size_t nStart = CStrW::end;

	do{
		if(strTemp.IsEmpty())
			break;

		DWORD dwResult = GetShortPathNameW(strTemp, szShort, MAX_PATH);
		if(dwResult > MAX_PATH)
			break;
		else if(dwResult)
		{
			if(str.CompareLeft(szShort, strTemp.GetLength(), true) != 0)
			{
				str.Delete(0, strTemp.GetLength());
				str.Prepend(szShort);
				bConverted = true;
			}
			break;
		}
		else
			dwErrorCode = GetLastError();

		size_t nPos = strTemp.ReverseFind(L'\\', nStart);
		if(nPos == 0 || nPos == CStrW::not_found)
			break;
		else
		{
			strTemp.CropLeft(nPos);
			nStart = nPos;
		}
	}while(dwErrorCode == ERROR_FILE_NOT_FOUND || dwErrorCode == ERROR_PATH_NOT_FOUND);

	return bConverted;
}

//#################################################################################################
bool ConvertToShort(CFilePathW &path)
{
	bool bConverted = false;

	CFilePathW pathTemp(path);

	size_t nSegmentCount = path.GetSegmentCount();
	DWORD dwErrorCode;

	do{
		if(pathTemp.IsEmpty())
			break;

		DWORD dwLen = GetShortPathNameW(pathTemp.Get(), nullptr, 0);
		if(dwLen)
		{
			auto szShort = std::make_unique<wchar_t[]>(dwLen);
			if(GetShortPathNameW(pathTemp.Get(), szShort.get(), dwLen))
			{
				path.TrimLeft(nSegmentCount);
				path.Prepend(CFilePath(szShort.get()));
				bConverted = true;
				break;
			}
			else
				dwErrorCode = GetLastError();
		}
		else
			dwErrorCode = GetLastError();

		pathTemp.TrimRight();
		--nSegmentCount;
	}while(dwErrorCode == ERROR_FILE_NOT_FOUND || dwErrorCode == ERROR_PATH_NOT_FOUND);

	return bConverted;
}

//#################################################################################################
bool CanonicalizeLocalUNC(CFilePathW &path)
{
	bool bConverted = false;

	if(path.IsUNC() && (path.GetMachine() == GetMachineName() || path.GetMachine() == L"localhost" || path.GetMachine() == L"127.0.0.1"))
	{
		PSHARE_INFO_2 psi2 = nullptr;
		NET_API_STATUS status;

		// Convert machine and share to a canonical path
		status = NetShareGetInfo(nullptr, (PWSTR)path.GetShare().Get(), 2, (PBYTE*)&psi2);
		if(status == NERR_Success)
		{
			CFilePathW pathRoot;
			if(pathRoot.Assign((PWSTR)psi2->shi2_path) == NO_ERROR)
			{
				path.TrimLeft(2);
				path.Prepend(pathRoot);
				bConverted = true;
			}
			NetApiBufferFree(psi2);
		}
	}

	return bConverted;
}
#endif

//#################################################################################################
bool CanonicalizeLink(CFilePath &path)
{
	bool bConverted = false;

#ifdef _WIN32
	// The following structure is in lieu of including <ntifs.h> which is a part of the DDK and does not work well in a user-mode application
	struct REPARSE_DATA_BUFFER final
	{
		ULONG ReparseTag;
		USHORT ReparseDataLength;
		USHORT Reserved;
		union
		{
			struct
			{
				USHORT SubstituteNameOffset;
				USHORT SubstituteNameLength;
				USHORT PrintNameOffset;
				USHORT PrintNameLength;
				ULONG Flags;
				WCHAR PathBuffer[1];
			} SymbolicLinkReparseBuffer;
			struct
			{
				USHORT SubstituteNameOffset;
				USHORT SubstituteNameLength;
				USHORT PrintNameOffset;
				USHORT PrintNameLength;
				WCHAR PathBuffer[1];
			} MountPointReparseBuffer;
			struct
			{
				UCHAR DataBuffer[1];
			} GenericReparseBuffer;
		} DUMMYUNIONNAME;
	};
	using PREPARSE_DATA_BUFFER = REPARSE_DATA_BUFFER*;
	// End ntifs.h substitute

	WIN32_FIND_DATAW w32fd;
	CStrW strPath;
	size_t nCurrentSegment = 1;

	while(nCurrentSegment <= path.GetSegmentCount())
	{
		strPath = path.GetLeft(nCurrentSegment);

		HANDLE hFind = FindFirstFileExW(strPath, GetFindInfoLevel(), &w32fd, FindExSearchNameMatch, nullptr, 0);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT) && (HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_MOUNT_POINT) || HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_SYMLINK)))
			{
				HANDLE hFile = CreateFileW(strPath, FILE_READ_EA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
				if(hFile != INVALID_HANDLE_VALUE)
				{
					auto pBuffer = std::make_unique<BYTE[]>(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
					PREPARSE_DATA_BUFFER pReparse = (PREPARSE_DATA_BUFFER)pBuffer.get();

					DWORD dwBytesRead;
					if(DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, nullptr, 0, pReparse, MAXIMUM_REPARSE_DATA_BUFFER_SIZE, &dwBytesRead, nullptr) && dwBytesRead)
					{
						if(IsReparseTagMicrosoft(pReparse->ReparseTag))
						{
							if(pReparse->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT)
							{
								strPath.Assign(pReparse->MountPointReparseBuffer.PathBuffer + pReparse->MountPointReparseBuffer.PrintNameOffset / sizeof(wchar_t), 0, pReparse->MountPointReparseBuffer.PrintNameLength / sizeof(wchar_t));
								path.TrimLeft(nCurrentSegment);
								path.Prepend(CFilePathW(strPath));
								nCurrentSegment = 1;
								bConverted = true;
								continue;
							}
							else if(pReparse->ReparseTag == IO_REPARSE_TAG_SYMLINK)
							{
								strPath.Assign(pReparse->SymbolicLinkReparseBuffer.PathBuffer + pReparse->SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(wchar_t), 0, pReparse->SymbolicLinkReparseBuffer.PrintNameLength / sizeof(wchar_t));
								path.TrimLeft(nCurrentSegment);
								path.Prepend(CFilePathW(strPath));
								nCurrentSegment = 1;
								bConverted = true;
								continue;
							}
						}
					}

					CloseHandle(hFile);
				}
			}

			FindClose(hFind);
		}

		++nCurrentSegment;
	}
#else
	CStr8 strPath;
	size_t nCurrentSegment = 1;
	auto szPath = std::make_unique<char[]>(MAX_PATH);

#ifdef __APPLE__
	size_t nEASize = sizeof(uint32_t) + sizeof(attrreference) + MAX_PATH;
	auto pBuffer = std::make_unique<BYTE[]>(nEASize);
	attrlist al = {0};
	al.bitmapcount = ATTR_BIT_MAP_COUNT;
	al.forkattr = ATTR_CMNEXT_NOFIRMLINKPATH;
#endif

	while(nCurrentSegment <= path.GetSegmentCount())
	{
		strPath = path.GetLeft(nCurrentSegment);
		ssize_t nResult = readlink(strPath, szPath.get(), MAX_PATH);
		if(nResult > 0)
		{
			Assert(nResult != MAX_PATH);
			szPath[nResult] = '\0';

			bool bAnchored = path.IsAnchored();
			path.TrimLeft(nCurrentSegment);
			path.Prepend(CFilePath(szPath.get()));
			if(bAnchored)
				path.SetAnchor(true);
			nCurrentSegment = 1;
			bConverted = true;
			continue;
		}

#ifdef __APPLE__
		if(getattrlist(strPath, &al, pBuffer.get(),nEASize, FSOPT_ATTR_CMN_EXTENDED) == 0)
		{
			int32_t nOffset;
			std::memcpy(&nOffset, pBuffer.get() + sizeof(uint32_t), sizeof(nOffset));
			CStr strTmp((PCSTR)(pBuffer.get() + sizeof(uint32_t) + nOffset));
			if(strTmp != strPath)
			{
				bool bAnchored = path.IsAnchored();
				path.TrimLeft(nCurrentSegment);
				path.Prepend(CFilePath(strTmp));
				if(bAnchored)
					path.SetAnchor(true);
				nCurrentSegment = 1;
				bConverted = true;
				continue;
			}
		}
#endif

		++nCurrentSegment;
	}
#endif

	return bConverted;
}

//#################################################################################################
bool CanonicalizeRelative(CFilePath &path)
{
	bool bConverted = false;

	if(!path.IsRelative())
		return bConverted;

#ifdef _WIN32
	if(path.IsAnchored())
	{	// A relative path on the current drive like '\windows\system32'
		if(path.Prepend(GetCurrentFolder().GetDrive()) == FW_NO_ERROR)
			bConverted = true;
	}
	else if(path.IsDrive())
	{	// A drive-relative path like 'd:temp'
		CFilePathSegmentW drive = path.GetDrive();
		path.TrimLeft();
		if(path.Prepend(GetCurrentFolder(drive)) == FW_NO_ERROR)
			bConverted = true;
	}
	else
	{	// A relative path off of the current working directory like 'files\drivers'
		if(path.Prepend(GetCurrentFolder()) == FW_NO_ERROR)
			bConverted = true;
	}
#else
	// A relative path off of the current working directory like 'files\drivers'
	if(path.Prepend(GetCurrentFolder()) == FW_NO_ERROR)
		bConverted = true;
#endif

	return bConverted;
}

//#################################################################################################
bool Canonicalize(CFilePath &path)
{
	bool bConverted = false;

#ifdef _WIN32
	if(CanonicalizeLocalUNC(path))
		bConverted = true;
#endif

	if(CanonicalizeRelative(path))
		bConverted = true;

	if(path.SimplifyDots())
		bConverted = true;

	if(CanonicalizeLink(path))
		bConverted = true;

#ifdef _WIN32
	if(ConvertToLong(path))
		bConverted = true;
#endif

	return bConverted;
}

NS_END
