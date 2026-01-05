#include "Base.h"
#include "FileTraverse.h"
#include "FileSystemUtils.h"
#ifndef _WIN32
#ifdef __APPLE__
#include <sys/attr.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#endif

NS_USE

static NCHAR g_chPeriod = _N('.');
#ifdef _WIN32
static NCHAR g_chAsterisk = _N('*');
#endif

//#################################################################################################
CFileTraverse::CFileTraverse(CEvent &event)
	: m_event(event),
	m_thread(CStr(_N("FileTraverser")))
#ifdef _WIN32
	, m_pathSearch(g_chAsterisk)
#endif
{
}

//#################################################################################################
void CFileTraverse::AddIncludeDirectory(const CFilePath &path)
{
	if(DirExists(path.Get()))
		m_qIncludeDirectories.push(path);
}

//#################################################################################################
void CFileTraverse::AddExcludeDirectory(const CFilePath &path)
{
	if(DirExists(path.Get()))
		m_setExcludeDirectories.insert(path);
}

#ifdef _WIN32
//#################################################################################################
void CFileTraverse::SetFilenameSearch(PCNSTR szSearch)
{
	m_pathSearch = szSearch;
	if(m_pathSearch.IsEmpty())
		m_pathSearch = g_chAsterisk;
}

//#################################################################################################
void CFileTraverse::SetFilenameSearch(const CFilePathSegment &pathSearch)
{
	if(pathSearch.IsEmpty())
		m_pathSearch = g_chAsterisk;
	else
		m_pathSearch = pathSearch;
}
#endif

//#################################################################################################
void CFileTraverse::Run(const bool bWait)
{
	Init();

	if(bWait)
		Wait();
}

//#################################################################################################
void CFileTraverse::Wait(void)
{
	m_thread.Wait();
}

//#################################################################################################
void CFileTraverse::Reset(void)
{
	Assert(!IsActive());

	while(!m_qIncludeDirectories.empty())
		m_qIncludeDirectories.pop();
	m_setExcludeDirectories.clear();
}

//#################################################################################################
bool CFileTraverse::IsActive(void) const noexcept
{
	return m_thread.IsActive();
}

//#################################################################################################
void CFileTraverse::Init(void)
{
	Free();

	m_thread.Start(TraverseThreadProc, this);
}

//#################################################################################################
void CFileTraverse::Free(void)
{
	m_thread.Wait();
	m_thread.Reset();
}

//#################################################################################################
void CFileTraverse::PreTraverse(void)
{
}

//#################################################################################################
void CFileTraverse::Traverse(void)
{
#ifdef _WIN32
	CFilePath pathBase;
	CFilePath pathSearch;
	CFilePath pathFound;
	HANDLE hFind = nullptr;
	WIN32_FIND_DATAW w32fd;

	while(!m_event.Wait(0))
	{
		if(hFind == nullptr)
		{
			if(m_qIncludeDirectories.empty())
				break;
			pathBase = m_qIncludeDirectories.front();
			m_qIncludeDirectories.pop();
			pathSearch = pathBase + m_pathSearch;

			hFind = FindFirstFileExW(pathSearch.Get(), GetFindInfoLevel(), &w32fd, FindExSearchNameMatch, nullptr, 0);
			if(hFind == INVALID_HANDLE_VALUE)
			{
				hFind = nullptr;
				continue;
			}
		}
		else
		{
			if(!FindNextFileW(hFind, &w32fd))
			{
				FindClose(hFind);
				hFind = nullptr;
				continue;
			}
		}

		pathFound = pathBase + CFilePathSegment(w32fd.cFileName);

		if(HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
		{	// Do not traverse into junctions or symbolic links
			// Do not add "dot" directories to the queue
			// Do not traverse into excluded folders
			if((!HAS_FLAG(w32fd.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT) || (!HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_MOUNT_POINT) && !HAS_FLAG(w32fd.dwReserved0, IO_REPARSE_TAG_SYMLINK))) &&
				(w32fd.cFileName[0] != g_chPeriod || (w32fd.cFileName[1] != g_chNullW && (w32fd.cFileName[1] != g_chPeriod || w32fd.cFileName[2] != g_chNullW))) &&
				m_setExcludeDirectories.find(pathFound) == m_setExcludeDirectories.end() && ProcessFolder(pathFound, w32fd))
			{
				AddIncludeDirectory(pathFound);
			}
		}
		else if(!ProcessFile(pathFound, w32fd))
			break;
	}

	if(hFind)
		FindClose(hFind);

#else

	CFilePath pathBase;
	CFilePath pathFound;
	CFilePath pathTmp;
	struct stat s;
	dev_t nStartDev = 0;
	bool bFirst = true;

#ifdef __APPLE__
	size_t nEASize = sizeof(uint32_t) + sizeof(attrreference) + PATH_MAX;
	auto pBuffer = std::make_unique<BYTE[]>(nEASize);
	attrlist al = {0};
	al.bitmapcount = ATTR_BIT_MAP_COUNT;
	al.forkattr = ATTR_CMNEXT_NOFIRMLINKPATH;
#endif

	while(!m_qIncludeDirectories.empty() && !m_event.Wait(0))
	{
		pathBase = m_qIncludeDirectories.front();
		m_qIncludeDirectories.pop();

		DIR *pDir = opendir(pathBase.Get());
		if(pDir)
		{
			struct dirent *pEntry = readdir(pDir);
			while(pEntry && !m_event.Wait(0))
			{
				pathFound = pathBase + CFilePathSegment8(pEntry->d_name);
				if(lstat(pathFound.Get(), &s) == 0 && !S_ISLNK(s.st_mode))
				{	// Do not process symbolic links
					// Do not traverse into folders with a different device Id
					// Do not add "dot" directories to the queue
					// Do not traverse into excluded folders

#ifdef __APPLE__
					// Do not traverse firmlinks
					bool bFirmlink = false;
					if(getattrlist(pathFound .Get(), &al, pBuffer.get(),nEASize, FSOPT_ATTR_CMN_EXTENDED) == 0)
					{
						int32_t nOffset;
						std::memcpy(&nOffset, pBuffer.get() + sizeof(uint32_t), sizeof(nOffset));
						pathTmp = (PCSTR)(pBuffer.get() + sizeof(uint32_t) + nOffset);
						bFirmlink = (pathTmp != pathFound);
					}

					if(!bFirmlink)
#endif
					{
						if(bFirst)
						{	// Record the starting device Id
							nStartDev = s.st_dev;
							bFirst = false;
						}

						if(nStartDev == s.st_dev)
						{
							if(S_ISDIR(s.st_mode))
							{
								if((pEntry->d_name[0] != g_chPeriod || (pEntry->d_name[1] != g_chNull && (pEntry->d_name[1] != g_chPeriod || pEntry->d_name[2] != g_chNull))) &&
									m_setExcludeDirectories.find(pathFound) == m_setExcludeDirectories.end())
								{
									if(!ProcessFolder(pathFound, s))
									{
										m_event.Signal();
										break;
									}

									AddIncludeDirectory(pathFound);
								}
							}
							else if(S_ISREG(s.st_mode) && !ProcessFile(pathFound, s))
							{
								m_event.Signal();
								break;
							}
						}
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
void CFileTraverse::PostTraverse(void)
{
}

#ifdef _WIN32
//#################################################################################################
bool CFileTraverse::ProcessFolder(const CFilePath &pathFolder, const WIN32_FIND_DATAW &w32fd)
{
	UNUSED(pathFolder);
	UNUSED(w32fd);

	return !m_event.Wait(0);
}

#else

//#################################################################################################
bool CFileTraverse::ProcessFolder(const CFilePath &pathFolder, const struct stat &s)
{
	UNUSED(pathFolder);
	UNUSED(s);

	return !m_event.Wait(0);
}
#endif

//#################################################################################################
void CFileTraverse::TraverseThreadProc(PVOID pParam)
{
	CFileTraverse *pThis = (CFileTraverse*)pParam;

	pThis->PreTraverse();
	pThis->Traverse();
	pThis->PostTraverse();
}
