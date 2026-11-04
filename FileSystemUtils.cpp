#include "Base.h"
#include "FileSystemUtils.h"
#include "Defer.h"
#include "Event.h"
#include "TimeUtils.h"
#include <queue>
#ifdef _WIN32
#include "File.h"
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
constexpr auto ATTRIB_MASK = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_OFFLINE | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
#else
constexpr auto COPY_BUFFER_SIZE = 8192;
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
		DEFER(FindClose(hFind));

		if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT) &&
			((HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_MOUNT_POINT)) ||	// Junction
			(HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_SYMLINK))))			// Symbolic link
		{
			bLink = true;
		}
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
	const size_t nTotalSegments = path.GetSegmentCount();

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
			DEFER(FindClose(hFind));

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
			DEFER(closedir(pDir));

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
			DEFER(FindClose(hFind));

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
			DEFER(closedir(pDir));

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
ERRCODE FileSetTime(PCNSTR szPath, const time_point<system_clock> &tp)
{
	ERRCODE nErrorCode;

#ifdef _WIN32
	CFile file;
	if((nErrorCode = file.Create(szPath, CFile::EFileMode::ExistingReadWrite)) == FW_NO_ERROR)
		nErrorCode = file.SetTime(tp);
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
ERRCODE FileSetAttributes(PCNSTR szPath, const DWORD dwAttrib)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert((dwAttrib & ~ATTRIB_MASK) == 0);

	if(!SetFileAttributesW(szPath, dwAttrib))
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
ERRCODE FileSetPermissions(PCNSTR szPath, const mode_t nMode)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert(!HAS_FLAG(nMode, S_IFMT));

	if(chmod(szPath, nMode) == -1)
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
		DEFER(close(nFileSrc));

		int nFileDest = open(szPathDest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(nFileDest != -1)
		{
			DEFER(close(nFileDest));

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
		DEFER(close(nFileSrc));

		int nFileDest = open(szPathDest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(nFileDest != -1)
		{
			DEFER(close(nFileDest));

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
		}
		else
			nErrorCode = ConvertFromNativeErrorCode(errno);

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
