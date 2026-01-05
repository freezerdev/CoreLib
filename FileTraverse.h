#pragma once

#include "StlHelper.h"
#include "Thread.h"
#include <queue>
#include <unordered_set>
#ifndef _WIN32
#include <sys/stat.h>
#endif

NS_USE

//#################################################################################################
class CFileTraverse
{
public:
	CFileTraverse(CEvent &event);

	// Classes derived from CFileTraverse must call Free() in their destructor
	virtual ~CFileTraverse(void) = default;

	void AddIncludeDirectory(const CFilePath &path);
	void AddExcludeDirectory(const CFilePath &path);

#ifdef _WIN32
	void SetFilenameSearch(PCNSTR szSearch);
	void SetFilenameSearch(const CFilePathSegment &pathSearch);
#endif

	virtual void Run(const bool bWait = false);
	virtual void Wait(void);
	virtual void Reset(void);

	bool IsActive(void) const noexcept;

protected:
	CEvent &m_event;

	void Init(void);
	void Free(void);

#ifdef _WIN32
	virtual bool ProcessFolder(const CFilePath &pathFolder, const WIN32_FIND_DATAW &w32fd);
	virtual bool ProcessFile(const CFilePath &pathFile, const WIN32_FIND_DATAW &w32fd) = 0;
#else
	virtual bool ProcessFolder(const CFilePath &pathFolder, const struct stat &s);
	virtual bool ProcessFile(const CFilePath &pathFile, const struct stat &s) = 0;
#endif

private:
	CThread m_thread;
	std::queue<CFilePath> m_qIncludeDirectories;
	std::unordered_set<CFilePath, CFilePathHashTraits> m_setExcludeDirectories;
#ifdef _WIN32
	CFilePathSegment m_pathSearch;
#endif

	virtual void PreTraverse(void);
	virtual void Traverse(void);
	virtual void PostTraverse(void);

	static void TraverseThreadProc(PVOID pParam);
};
