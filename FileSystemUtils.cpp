#include "Base.h"
#include "FileSystemUtils.h"
#include "Event.h"
#include "TimeUtils.h"
#include <queue>
#ifdef _WIN32
#include <versionhelpers.h>
#else
#ifdef __APPLE__
#include <sys/attr.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#endif

NS_BEGIN

#ifdef _WIN32
static constexpr auto ATTRIB_MASK = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_OFFLINE | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
#else
static constexpr auto COPY_BUFFER_SIZE = 8192;
#endif

#ifdef _WIN32
static FINDEX_INFO_LEVELS g_fexil = FindExInfoStandard;

//#################################################################################################
void FileSystemInit(void)
{
	if(IsWindowsVersionOrGreater(6, 1, 0))
		g_fexil = FindExInfoBasic;
}

//#################################################################################################
FINDEX_INFO_LEVELS GetFindInfoLevel(void) noexcept
{
	return g_fexil;
}
#endif

//#################################################################################################
NCHAR GetDelimiter(void) noexcept
{
#ifdef _WIN32
	return L'\\';
#else
	return '/';
#endif
}

//#################################################################################################
bool IsLink(PCNSTR szPath)
{
	bool bLink = false;

#ifdef _WIN32
	WIN32_FIND_DATAW w32fd;
	HANDLE hFind = FindFirstFileExW(szPath, g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT) &&
			((HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_MOUNT_POINT)) ||	// Junction
			(HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_SYMLINK))))			// Symbolic link
		{
			bLink = true;
		}

		FindClose(hFind);
	}
#else
	struct stat s;
	bLink = (lstat(szPath, &s) == 0 && S_ISLNK(s.st_mode));
#ifdef __APPLE__
	if(!bLink)
	{
		size_t nEASize = sizeof(uint32_t) + sizeof(attrreference) + PATH_MAX;
		auto pBuffer = std::make_unique<BYTE[]>(nEASize);
		attrlist al = {0};
		al.bitmapcount = ATTR_BIT_MAP_COUNT;
		al.forkattr = ATTR_CMNEXT_NOFIRMLINKPATH;

		if(getattrlist(szPath, &al, pBuffer.get(),nEASize, FSOPT_ATTR_CMN_EXTENDED) == 0)
		{
			int32_t nOffset;
			std::memcpy(&nOffset, pBuffer.get() + sizeof(uint32_t), sizeof(nOffset));
			CStr strOrig(szPath);
			CStr strTmp((PCSTR)(pBuffer.get() + sizeof(uint32_t) + nOffset));
			bLink = (strTmp.Compare(strOrig) != 0);
		}
	}
#endif
#endif

	return bLink;
}

//#################################################################################################
bool DirExists(PCNSTR szPath)
{
	bool bExists = false;

#ifdef _WIN32
	WIN32_FILE_ATTRIBUTE_DATA w32fad;
	if(GetFileAttributesExW(szPath, GetFileExInfoStandard, &w32fad) && HAS_FLAG(w32fad.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
		bExists = true;
#else
	struct stat s;
	bExists = (stat(szPath, &s) == 0 && S_ISDIR(s.st_mode));
#endif

	return bExists;
}

//#################################################################################################
ERRCODE DirCreate(PCNSTR szPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(!CreateDirectoryW(szPath, nullptr))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(mkdir(szPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE DirDelete(PCNSTR szPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(!RemoveDirectoryW(szPath))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(rmdir(szPath) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
bool DirCreateTree(const CFilePath &path)
{
	if(path.IsEmpty())
		return false;

	CFilePath pathTemp;
	size_t nCurrentSegment = 1;
	size_t nTotalSegments = path.GetSegmentCount();

	if(path.IsUNC())
		nCurrentSegment = MIN(2, nTotalSegments);

	while(nCurrentSegment <= nTotalSegments)
	{
		pathTemp = path.GetLeft(nCurrentSegment);
		if(!DirExists(pathTemp.Get()))
		{
			if(DirCreate(pathTemp.Get()) != FW_NO_ERROR)
				return false;
		}
		++nCurrentSegment;
	}

	return true;
}

//#################################################################################################
bool DirDeleteTree(const CFilePath &path)
{
	bool bResult = true;

	// Enumerate all child files and directories
	std::vector<CFilePath> vecFiles;
	std::vector<CFilePath> vecDirs;
	DirEnum(path, &vecFiles, &vecDirs, true);

	// Remove the child files
	for(const auto &itr : vecFiles)
	{
		if(FileDelete(itr.Get()) != FW_NO_ERROR)
			bResult = false;
	}

	// Remove the child directories
	for(auto itr = vecDirs.rbegin(); itr != vecDirs.rend(); ++itr)
	{
		if(DirDelete(itr->Get()) != FW_NO_ERROR)
			bResult = false;
	}

	// Remove the parent directory
	if(DirDelete(path.Get()) != FW_NO_ERROR)
		bResult = false;

	return bResult;
}

//#################################################################################################
uint64_t DirGetSize(const CFilePath &path, CEvent *pAbort)
{
	uint64_t nSize = 0;

#ifdef _WIN32
	CFilePathW pathBase;
	CFilePathW pathSearch;
	CFilePathW pathFound;
	std::queue<CFilePathW> qPaths;
	WIN32_FIND_DATAW w32fd;

	if(!path.IsEmpty())
		qPaths.push(path);

	while(!qPaths.empty() && (pAbort == nullptr || pAbort->Wait(0) == 0))
	{
		pathBase = qPaths.front();
		qPaths.pop();
		pathSearch = pathBase + CFilePathSegmentW(L'*');

		HANDLE hFind = FindFirstFileExW(pathSearch.Get(), g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do{
				pathFound = pathBase + CFilePathSegmentW(w32fd.cFileName);

				if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
				{	// Do not traverse into junctions or symbolic links
					// Do not add "dot" directories to the queue
					// Do not traverse into excluded folders
					if((!HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT) || (!HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_MOUNT_POINT) && !HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_SYMLINK))) &&
						(w32fd.cFileName[0] != L'.' || (w32fd.cFileName[1] != g_chNullW && (w32fd.cFileName[1] != L'.' || w32fd.cFileName[2] != g_chNullW))))
					{
						qPaths.push(std::move(pathFound));
					}
				}
				else
					nSize += MAKEUINT64(w32fd.nFileSizeHigh, w32fd.nFileSizeLow);
			}while(FindNextFileW(hFind, &w32fd) && (pAbort == nullptr || pAbort->Wait(0) == 0));
			FindClose(hFind);
		}
	}
#else
	CFilePath8 pathBase;
	CFilePath8 pathFound;
	std::queue<CFilePath8> qPaths;
	struct stat s;
	dev_t nStartDev = 0;
	bool bFirst = true;

	if(!path.IsEmpty())
		qPaths.push(path);

	while(!qPaths.empty() && (pAbort == nullptr || pAbort->Wait(0) == 0))
	{
		pathBase = qPaths.front();
		qPaths.pop();

		DIR *pDir = opendir(pathBase.Get());
		if(pDir)
		{
			struct dirent *pEntry = readdir(pDir);
			while(pEntry && (pAbort == nullptr || pAbort->Wait(0) == 0))
			{
				pathFound = pathBase + CFilePathSegment8(pEntry->d_name);
				if(lstat(pathFound.Get(), &s) == 0 && !S_ISLNK(s.st_mode))
				{	// Do not process symbolic links
					// Do not traverse into folders with a different device Id
					// Do not add "dot" directories to the queue
					// Do not traverse into excluded folders
					// Note: It does traverse into firmlinks
					if(bFirst)
					{	// Record the starting device Id
						nStartDev = s.st_dev;
						bFirst = false;
					}

					if(nStartDev == s.st_dev)
					{
						if(S_ISDIR(s.st_mode))
						{
							if(pEntry->d_name[0] != '.' || (pEntry->d_name[1] != '\0' && (pEntry->d_name[1] != '.' || pEntry->d_name[2] != '\0')))
								qPaths.push(std::move(pathFound));
						}
						else if(S_ISREG(s.st_mode))
							nSize += (uint64_t)s.st_size;
					}
				}

				pEntry = readdir(pDir);
			}

			closedir(pDir);
		}
	}
#endif

	return nSize;
}

//#################################################################################################
void DirEnum(const CFilePath &path, std::vector<CFilePath> *pvecFiles, std::vector<CFilePath> *pvecDirs, const bool bRecursive, CEvent *pAbort)
{
	if(pvecFiles)
		pvecFiles->clear();

	if(pvecDirs)
		pvecDirs->clear();

#ifdef _WIN32
	CFilePathW pathBase;
	CFilePathW pathSearch;
	CFilePathW pathFound;
	std::queue<CFilePathW> qPaths;
	WIN32_FIND_DATAW w32fd;

	if(!path.IsEmpty())
		qPaths.push(path);

	while(!qPaths.empty() && (pAbort == nullptr || pAbort->Wait(0) == 0))
	{
		pathBase = qPaths.front();
		qPaths.pop();
		pathSearch = pathBase + CFilePathSegmentW(L'*');

		HANDLE hFind = FindFirstFileExW(pathSearch.Get(), g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do{
				pathFound = pathBase + CFilePathSegmentW(w32fd.cFileName);

				if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
				{	// Do not traverse into junctions or symbolic links
					// Do not add "dot" directories to the queue
					// Do not traverse into excluded folders
					if((!HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT) || (!HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_MOUNT_POINT) && !HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_SYMLINK))) &&
						(w32fd.cFileName[0] != L'.' || (w32fd.cFileName[1] != g_chNullW && (w32fd.cFileName[1] != L'.' || w32fd.cFileName[2] != g_chNullW))))
					{
						if(pvecDirs)
							pvecDirs->push_back(pathFound);

						if(bRecursive)
							qPaths.push(std::move(pathFound));
					}
				}
				else if(pvecFiles)
					pvecFiles->push_back(std::move(pathFound));
			}while(FindNextFileW(hFind, &w32fd) && (pAbort == nullptr || pAbort->Wait(0) == 0));
			FindClose(hFind);
		}
	}
#else
	CFilePath8 pathBase;
	CFilePath8 pathFound;
	std::queue<CFilePath8> qPaths;
	struct stat s;
	dev_t nStartDev = 0;
	bool bFirst = true;

	if(!path.IsEmpty())
		qPaths.push(path);

	while(!qPaths.empty())
	{
		pathBase = qPaths.front();
		qPaths.pop();

		DIR *pDir = opendir(pathBase.Get());
		if(pDir)
		{
			struct dirent *pEntry = readdir(pDir);
			while(pEntry)
			{
				pathFound = pathBase + CFilePathSegment8(pEntry->d_name);
				if(lstat(pathFound.Get(), &s) == 0 && !S_ISLNK(s.st_mode))
				{	// Do not process symbolic links
					// Do not traverse into folders with a different device Id
					// Do not add "dot" directories to the queue
					// Do not traverse into excluded folders
					// Note: It does traverse into firmlinks
					if(bFirst)
					{	// Record the starting device Id
						nStartDev = s.st_dev;
						bFirst = false;
					}

					if(nStartDev == s.st_dev)
					{
						if(S_ISDIR(s.st_mode))
						{
							if(pEntry->d_name[0] != '.' || (pEntry->d_name[1] != '\0' && (pEntry->d_name[1] != '.' || pEntry->d_name[2] != '\0')))
							{
								if(pvecDirs)
									pvecDirs->push_back(pathFound);

								if(bRecursive)
									qPaths.push(std::move(pathFound));
							}
						}
						else if(S_ISREG(s.st_mode) && pvecFiles)
							pvecFiles->push_back(std::move(pathFound));
					}
				}

				pEntry = readdir(pDir);
			}

			closedir(pDir);
		}
	}
#endif
}

//#################################################################################################
bool FileExists(PCNSTR szPath)
{
	bool bExists = false;

#ifdef _WIN32
	WIN32_FIND_DATAW w32fd;
	HANDLE hFind = FindFirstFileExW(szPath, g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		if(!HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
			bExists = true;
	}
#else
	struct stat s;
	bExists = (stat(szPath, &s) == 0 && S_ISREG(s.st_mode));
#endif

	return bExists;
}

//#################################################################################################
ERRCODE FileCreate(PCNSTR szPath, const EFileMode eMode, NHANDLE &hFile)
{
	Assert(hFile == INVALID_NHANDLE);

	ERRCODE nErrorCode = FW_NO_ERROR;

	hFile = INVALID_NHANDLE;

	if(szPath == nullptr || *szPath == _N('\0'))
		return FW_ERROR_INVALID_PARAMETER;

#ifdef _WIN32
	DWORD dwAccess = 0;
	DWORD dwShare = FILE_SHARE_READ;
	DWORD dwDisposition = 0;
	switch(eMode)
	{
	case EFM_ExistingReadOnly:
		dwAccess = GENERIC_READ;
		ADD_FLAG(dwShare, FILE_SHARE_WRITE | FILE_SHARE_DELETE);
		dwDisposition = OPEN_EXISTING;
		break;

	case EFM_ExistingReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwDisposition = OPEN_EXISTING;
		break;

	case EFM_AlwaysReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwDisposition = OPEN_ALWAYS;
		break;

	case EFM_CreateWriteOnly:
		dwAccess = GENERIC_WRITE;
		ADD_FLAG(dwShare, FILE_SHARE_WRITE | FILE_SHARE_DELETE);
		dwDisposition = CREATE_ALWAYS;
		break;

	case EFM_CreateReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwDisposition = CREATE_ALWAYS;
		break;
	}

	hFile = CreateFileW(szPath, dwAccess, dwShare, nullptr, dwDisposition, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
		hFile = INVALID_NHANDLE;
	}
#else
	int nFlags = 0;
	switch(eMode)
	{
	case EFM_ExistingReadOnly:
		nFlags = O_RDONLY;
		break;

	case EFM_ExistingReadWrite:
		nFlags = O_RDWR;
		break;

	case EFM_AlwaysReadWrite:
		nFlags = O_RDWR | O_CREAT;
		break;

	case EFM_CreateWriteOnly:
		nFlags = O_WRONLY | O_CREAT | O_TRUNC;
		break;

	case EFM_CreateReadWrite:
		nFlags = O_RDWR | O_CREAT | O_TRUNC;
		break;
	}

	hFile = open(szPath, nFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(hFile == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileClose(const NHANDLE hFile)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(hFile != INVALID_NHANDLE && !CloseHandle(hFile))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(hFile != INVALID_NHANDLE && close(hFile) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileDelete(PCNSTR szPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(!DeleteFileW(szPath))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(unlink(szPath) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileForceDelete(PCNSTR szPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	WIN32_FIND_DATAW w32fd;
	HANDLE hFind = FindFirstFileExW(szPath, g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_READONLY) && !HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
			SetFileAttributesW(szPath, w32fd.dwFileAttributes & (~FILE_ATTRIBUTE_READONLY));
	}

	if(!DeleteFileW(szPath))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(unlink(szPath) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileRead(const NHANDLE hFile, PVOID pBuffer, const size_t nBytesToRead, size_t &nBytesRead)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nBytesRead = 0;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	DWORD dwBytesToRead = (DWORD)nBytesToRead;
	DWORD dwBytesRead;
	if(ReadFile(hFile, pBuffer, dwBytesToRead, &dwBytesRead, nullptr))
	{
		nBytesRead = (size_t)dwBytesRead;
		if(nBytesRead == 0)
			nErrorCode = FW_ERROR_HANDLE_EOF;
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	ssize_t nResult = read(hFile, pBuffer, nBytesToRead);
	if(nResult >= 0)
	{
		nBytesRead = (size_t)nResult;
		if(nBytesRead == 0)
			nErrorCode = FW_ERROR_HANDLE_EOF;
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileWrite(const NHANDLE hFile, PCVOID pBuffer, const size_t nBytesToWrite, size_t &nBytesWritten)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nBytesWritten = 0;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	DWORD dwBytesToWrite = (DWORD)nBytesToWrite;
	DWORD dwBytesWritten;
	if(WriteFile(hFile, pBuffer, dwBytesToWrite, &dwBytesWritten, nullptr))
		nBytesWritten = (size_t)dwBytesWritten;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	ssize_t nResult = write(hFile, pBuffer, nBytesToWrite);
	if(nResult >= 0)
		nBytesWritten = (size_t)nResult;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileFlush(const NHANDLE hFile)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	if(!FlushFileBuffers(hFile))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(fsync(hFile) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetPosition(const NHANDLE hFile, uint64_t &nOffset)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nOffset = 0;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	LARGE_INTEGER liMove = {0};
	LARGE_INTEGER liCurrent;
	if(SetFilePointerEx(hFile, liMove, &liCurrent, FILE_CURRENT))
		nOffset = liCurrent.QuadPart;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	off_t nCurrent = lseek(hFile, 0, SEEK_CUR);
	if(nCurrent != -1)
		nOffset = (uint64_t)nCurrent;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetPosition(const NHANDLE hFile, const int64_t nOffset, EFileSeek eSeek)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	LARGE_INTEGER liMove;
	liMove.QuadPart = nOffset;

	int nMode = FILE_BEGIN;
	switch(eSeek)
	{
	case EFS_Begin:
		nMode = FILE_BEGIN;
		break;

	case EFS_Current:
		nMode = FILE_CURRENT;
		break;

	case EFS_End:
		nMode = FILE_END;
		break;
	}

	if(!SetFilePointerEx(hFile, liMove, nullptr, nMode))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	int nSeek;
	switch(eSeek)
	{
	case EFS_Begin:
		nSeek = SEEK_SET;
		break;

	case EFS_Current:
		nSeek = SEEK_CUR;
		break;

	case EFS_End:
		nSeek = SEEK_END;
		break;
	}

	if(lseek(hFile, nOffset, nSeek) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetEnd(const NHANDLE hFile)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	if(!SetEndOfFile(hFile))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	off_t nOffset = lseek(hFile, 0, SEEK_CUR);
	if(nOffset != -1)
	{
		if(ftruncate(hFile, nOffset) == -1)
			nErrorCode = ConvertFromNativeErrorCode(errno);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetSize(PCNSTR szPath, uint64_t &nSize)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nSize = 0;

#ifdef _WIN32
	WIN32_FIND_DATAW w32fd;
	HANDLE hFind = FindFirstFileExW(szPath, g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		nSize = MAKEUINT64(w32fd.nFileSizeHigh, w32fd.nFileSizeLow);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	struct stat s;
	if(stat(szPath, &s) == 0)
		nSize = (uint64_t)s.st_size;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetSize(const NHANDLE hFile, uint64_t &nSize)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nSize = 0;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	LARGE_INTEGER li;
	if(GetFileSizeEx(hFile, &li))
		nSize = li.QuadPart;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	off_t nCurrent = lseek(hFile, 0, SEEK_CUR);
	if(nCurrent != -1)
	{
		off_t nEnd = lseek(hFile, 0, SEEK_END);
		if(nEnd != -1)
		{
			nSize = (uint64_t)nEnd;

			if(lseek(hFile, nCurrent, SEEK_SET) == -1)
				nErrorCode = ConvertFromNativeErrorCode(errno);
		}
		else
			nErrorCode = ConvertFromNativeErrorCode(errno);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetTime(PCNSTR szPath, time_point<system_clock> &tp)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	tp = time_point<system_clock>();

#ifdef _WIN32
	WIN32_FIND_DATAW w32fd;
	HANDLE hFind = FindFirstFileExW(szPath, g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		FileTimeToTimePoint(w32fd.ftLastWriteTime, tp);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	struct stat s;
	if(stat(szPath, &s) == 0)
		tp = system_clock::from_time_t(s.st_mtime);
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetTime(const NHANDLE hFile, time_point<system_clock> &tp)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	tp = time_point<system_clock>();

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	FILE_BASIC_INFO fbi = {0};
	if(GetFileInformationByHandleEx(hFile, FileBasicInfo, &fbi, sizeof(fbi)))
	{
		FILETIME ft;
		ft.dwHighDateTime = fbi.LastWriteTime.HighPart;
		ft.dwLowDateTime = fbi.LastWriteTime.LowPart;
		FileTimeToTimePoint(ft, tp);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	struct stat s;
	if(fstat(hFile, &s) == 0)
		tp = system_clock::from_time_t(s.st_mtime);
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetTime(PCNSTR szPath, const time_point<system_clock> &tp)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	HANDLE hFile = CreateFileW(szPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME ft;
		TimePointToFileTime(tp, ft);
		if(!SetFileTime(hFile, nullptr, nullptr, &ft))
			nErrorCode = ConvertFromNativeErrorCode(GetLastError());
		CloseHandle(hFile);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	int hFile = open(szPath, O_RDWR);
	if(hFile == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
	else
	{
		nErrorCode = FileSetTime(hFile, tp);
		close(hFile);
	}
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetTime(const NHANDLE hFile, const time_point<system_clock> &tp)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	FILETIME ft;
	TimePointToFileTime(tp, ft);
	if(!SetFileTime(hFile, nullptr, nullptr, &ft))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#elif __APPLE__
	auto ns = tp.time_since_epoch() - seconds(tp.time_since_epoch() / seconds(1));

	timeval times[2];
	times[0].tv_sec = times[1].tv_sec = system_clock::to_time_t(tp);
	times[0].tv_usec = times[1].tv_usec = (int)(ns.count() / 1000);
	if(futimes(hFile, times) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#elif __linux__
	auto ns = tp.time_since_epoch() - seconds(tp.time_since_epoch() / seconds(1));

	struct timespec times[2];
	times[0].tv_sec = times[1].tv_sec = system_clock::to_time_t(tp);
	times[0].tv_nsec = times[1].tv_nsec = ns.count();
	if(futimens(hFile, times) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetInfo(PCNSTR szPath, uint64_t &nSize, time_point<system_clock> &tp)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nSize = 0;
	tp = time_point<system_clock>();

#ifdef _WIN32
	WIN32_FIND_DATAW w32fd;
	HANDLE hFind = FindFirstFileExW(szPath, g_fexil, &w32fd, FindExSearchNameMatch, nullptr, 0);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		nSize = MAKEUINT64(w32fd.nFileSizeHigh, w32fd.nFileSizeLow);
		FileTimeToTimePoint(w32fd.ftLastWriteTime, tp);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	struct stat s;
	if(stat(szPath, &s) == 0)
	{
		nSize = (uint64_t)s.st_size;
		tp = system_clock::from_time_t(s.st_mtime);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetInfo(const NHANDLE hFile, uint64_t &nSize, time_point<system_clock> &tp)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nSize = 0;
	tp = time_point<system_clock>();

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	BY_HANDLE_FILE_INFORMATION bhfi = {0};
	if(GetFileInformationByHandle(hFile, &bhfi))
	{
		nSize = MAKEUINT64(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
		FileTimeToTimePoint(bhfi.ftLastWriteTime, tp);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	struct stat s;
	if(fstat(hFile, &s) == 0)
	{
		nSize = (uint64_t)s.st_size;
		tp = system_clock::from_time_t(s.st_mtime);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

#ifdef _WIN32
//#################################################################################################
ERRCODE FileGetAttributes(PCNSTR szPath, DWORD &dwAttrib)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	dwAttrib = 0;

	WIN32_FILE_ATTRIBUTE_DATA w32fad;
	if(GetFileAttributesExW(szPath, GetFileExInfoStandard, &w32fad))
		dwAttrib = w32fad.dwFileAttributes;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetAttributes(const NHANDLE hFile, DWORD &dwAttrib)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	dwAttrib = 0;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	FILE_BASIC_INFO fbi = {0};
	if(GetFileInformationByHandleEx(hFile, FileBasicInfo, &fbi, sizeof(fbi)))
		dwAttrib = fbi.FileAttributes;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetAttributes(PCNSTR szPath, const DWORD dwAttrib)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert((dwAttrib & ~ATTRIB_MASK) == 0);

	if(!SetFileAttributesW(szPath, dwAttrib))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetAttributes(const NHANDLE hFile, const DWORD dwAttrib)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert((dwAttrib & ~ATTRIB_MASK) == 0);

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	FILE_BASIC_INFO fbi = {0};
	if(GetFileInformationByHandleEx(hFile, FileBasicInfo, &fbi, sizeof(fbi)))
	{
		REMOVE_FLAG(fbi.FileAttributes, ATTRIB_MASK);
		ADD_FLAG(fbi.FileAttributes, dwAttrib);
		if(!SetFileInformationByHandle(hFile, FileBasicInfo, &fbi, sizeof(fbi)))
			nErrorCode = ConvertFromNativeErrorCode(GetLastError());
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

#else

//#################################################################################################
ERRCODE FileGetPermissions(PCNSTR szPath, mode_t &nMode)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nMode = 0;

	struct stat s;
	if(stat(szPath, &s) == 0)
		nMode = s.st_mode & ~S_IFMT;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileGetPermissions(const NHANDLE hFile, mode_t &nMode)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nMode = 0;

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	struct stat s;
	if(fstat(hFile, &s) == 0)
		nMode = s.st_mode & ~S_IFMT;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetPermissions(PCNSTR szPath, const mode_t nMode)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert(!HAS_FLAG(nMode, S_IFMT));

	if(chmod(szPath, nMode) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileSetPermissions(const NHANDLE hFile, const mode_t nMode)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert(!HAS_FLAG(nMode, S_IFMT));

	if(hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	if(fchmod(hFile, nMode) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}
#endif

//#################################################################################################
ERRCODE FileCopy(PCNSTR szPathSrc, PCNSTR szPathDest)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(!CopyFileW(szPathSrc, szPathDest, false))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	uint64_t nRemainingSize = 0;
	struct stat s;
	if(stat(szPathSrc, &s) == 0)
		nRemainingSize = (uint64_t)s.st_size;

	int nFileSrc = open(szPathSrc, O_RDONLY);
	if(nFileSrc != -1)
	{
		int nFileDest = open(szPathDest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(nFileDest != -1)
		{
			if(nRemainingSize != 0)
			{
				auto pBuffer = std::make_unique<BYTE[]>(COPY_BUFFER_SIZE);
				while(nRemainingSize)
				{
					size_t nBytesRead;

					ssize_t nResult = read(nFileSrc, pBuffer.get(), COPY_BUFFER_SIZE);
					if(nResult == 0)
						break;
					else if(nResult > 0)
						nBytesRead = (size_t)nResult;
					else
					{
						nErrorCode = ConvertFromNativeErrorCode(errno);
						break;
					}

					nResult = write(nFileDest, pBuffer.get(), nBytesRead);
					if(nResult >= 0)
					{
						size_t nBytesWritten = (size_t)nResult;
						nRemainingSize -= nBytesWritten;
						if(nBytesWritten != nBytesRead)
							break;
					}
					else
					{
						nErrorCode = ConvertFromNativeErrorCode(errno);
						break;
					}
				}

				if(nRemainingSize != 0)
					nErrorCode = FW_ERROR_INTERRUPTED;
			}

			close(nFileDest);
		}
		else
			nErrorCode = ConvertFromNativeErrorCode(errno);

		close(nFileSrc);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE FileMove(PCNSTR szPathSrc, PCNSTR szPathDest)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(!MoveFileExW(szPathSrc, szPathDest, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	uint64_t nRemainingSize = 0;
	struct stat s;
	if(stat(szPathSrc, &s) == 0)
		nRemainingSize = (uint64_t)s.st_size;

	int nFileSrc = open(szPathSrc, O_RDONLY);
	if(nFileSrc != -1)
	{
		int nFileDest = open(szPathDest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(nFileDest != -1)
		{
			if(nRemainingSize != 0)
			{
				auto pBuffer = std::make_unique<BYTE[]>(COPY_BUFFER_SIZE);
				while(nRemainingSize)
				{
					size_t nBytesRead;

					ssize_t nResult = read(nFileSrc, pBuffer.get(), COPY_BUFFER_SIZE);
					if(nResult == 0)
						break;
					else if(nResult > 0)
						nBytesRead = (size_t)nResult;
					else
					{
						nErrorCode = ConvertFromNativeErrorCode(errno);
						break;
					}

					nResult = write(nFileDest, pBuffer.get(), nBytesRead);
					if(nResult >= 0)
					{
						size_t nBytesWritten = (size_t)nResult;
						nRemainingSize -= nBytesWritten;
						if(nBytesWritten != nBytesRead)
							break;
					}
					else
					{
						nErrorCode = ConvertFromNativeErrorCode(errno);
						break;
					}
				}

				if(nRemainingSize != 0)
					nErrorCode = FW_ERROR_INTERRUPTED;
			}

			close(nFileDest);
		}
		else
			nErrorCode = ConvertFromNativeErrorCode(errno);

		close(nFileSrc);

		if(nErrorCode == FW_NO_ERROR)
		{
			if(unlink(szPathSrc) == -1)
				nErrorCode = ConvertFromNativeErrorCode(errno);
		}
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

NS_END
