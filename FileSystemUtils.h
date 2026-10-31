#pragma once

#include <vector>

NS_BEGIN

// Forward declarations
class CEvent;

#ifdef _WIN32
void FileSystemInit(void);
FINDEX_INFO_LEVELS GetFindInfoLevel(void) noexcept;
#endif

// Returns the delimiter for the current platform
NCHAR GetDelimiter(void) noexcept;
// Returns if the file is a symbolic link or junction
bool IsLink(PCNSTR szPath);

// Returns whether the given path exists as a directory
bool DirExists(PCNSTR szPath);
// Creates a new directory, only one level of directory can be created at a time
ERRCODE DirCreate(PCNSTR szPath);
// Removes a directory from the file system, must be an empty directory
ERRCODE DirDelete(PCNSTR szPath);

// Creates a directory, all levels of directory are created if they do not already exist
bool DirCreateTree(const CFilePath &path);
// Removes a directory from the file system, including all child files and directories
bool DirDeleteTree(const CFilePath &path);

// Returns the total size of all the files in the directory (recursively)
uint64_t DirGetSize(const CFilePath &path, CEvent *pAbort = nullptr);

// Enumerates files/directories in the given path, optionally recursively
void DirEnum(const CFilePath &path, std::vector<CFilePath> *pvecFiles, std::vector<CFilePath> *pvecDirs = nullptr, const bool bRecursive = false, CEvent *pAbort = nullptr);

// Returns whether the given path exists as a file
bool FileExists(PCNSTR szPath);

// Removes a file from the file system
ERRCODE FileDelete(PCNSTR szPath);
// Forces removal of a file from the file system, clearing read-only attribute if necessary
ERRCODE FileForceDelete(PCNSTR szPath);

// Returns the file size given the path or open file handle
ERRCODE FileGetSize(PCNSTR szPath, uint64_t &nSize);

// Returns the file modification time given the path or open file handle
ERRCODE FileGetTime(PCNSTR szPath, time_point<system_clock> &tp);

// Sets the file modification time given the path or open file handle
ERRCODE FileSetTime(PCNSTR szPath, const time_point<system_clock> &tp);

// Returns the file size and modification time given the path or open file handle
ERRCODE FileGetInfo(PCNSTR szPath, uint64_t &nSize, time_point<system_clock> &tp);

#ifdef _WIN32
ERRCODE FileGetAttributes(PCNSTR szPath, DWORD &dwAttrib);
ERRCODE FileSetAttributes(PCNSTR szPath, const DWORD dwAttrib);
#else
ERRCODE FileGetPermissions(PCNSTR szPath, mode_t &nMode);
ERRCODE FileSetPermissions(PCNSTR szPath, const mode_t nMode);
#endif

// Copies a file from the source to the destination, overwriting if it exists
ERRCODE FileCopy(PCNSTR szPathSrc, PCNSTR szPathDest);
// Moves a file from the source to the destination, overwriting if it exists
ERRCODE FileMove(PCNSTR szPathSrc, PCNSTR szPathDest);

NS_END
