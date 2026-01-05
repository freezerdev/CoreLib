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

enum EFileMode : uint8_t
{
	EFM_ExistingReadOnly = 0,	// Open existing file for read only
	EFM_ExistingReadWrite,		// Open existing file for read/write
	EFM_AlwaysReadWrite,		// Open existing file (or create new if not existing) for read/write
	EFM_CreateWriteOnly,		// Create new file for write only (overwrites existing)
	EFM_CreateReadWrite			// Create new file for read/write (overwrites existing)
};

// Opens an existing file or creates a new file, returns FW_NO_ERROR on success or an error code
ERRCODE FileCreate(PCNSTR szPath, const EFileMode eMode, NHANDLE &hFile);
// Closes an open file handle
ERRCODE FileClose(const NHANDLE hFile);
// Removes a file from the file system
ERRCODE FileDelete(PCNSTR szPath);
// Forces removal of a file from the file system, clearing read-only attribute if necessary
ERRCODE FileForceDelete(PCNSTR szPath);

// Reads 'nBytesToRead' of data from the open file handle and copies the data to 'pBuffer', the number of bytes read is returned in 'nBytesRead'
ERRCODE FileRead(const NHANDLE hFile, PVOID pBuffer, const size_t nBytesToRead, size_t &nBytesRead);
// Writes 'nBytesToWrite' of data to the open file handle from 'pBuffer', the number of bytes written is returned in 'nBytesWritten'
ERRCODE FileWrite(const NHANDLE hFile, PCVOID pBuffer, const size_t nBytesToWrite, size_t &nBytesWritten);
// Flushes data to disk
ERRCODE FileFlush(const NHANDLE hFile);

enum EFileSeek : uint8_t
{
	EFS_Begin = 0,				// Seek from the beginning of the file
	EFS_Current,				// Seek from the current position
	EFS_End						// Seek from the end of the file
};

// Gets the current file position
ERRCODE FileGetPosition(const NHANDLE hFile, uint64_t &nOffset);
// Sets the current file position relative to 'eSeek'
ERRCODE FileSetPosition(const NHANDLE hFile, const int64_t nOffset, const EFileSeek eSeek = EFS_Current);

// Sets the current file position as the end of the file (truncates any file after current position)
ERRCODE FileSetEnd(const NHANDLE hFile);

// Returns the file size given the path or open file handle
ERRCODE FileGetSize(PCNSTR szPath, uint64_t &nSize);
ERRCODE FileGetSize(const NHANDLE hFile, uint64_t &nSize);

// Returns the file modification time given the path or open file handle
ERRCODE FileGetTime(PCNSTR szPath, time_point<system_clock> &tp);
ERRCODE FileGetTime(const NHANDLE hFile, time_point<system_clock> &tp);

// Sets the file modification time given the path or open file handle
ERRCODE FileSetTime(PCNSTR szPath, const time_point<system_clock> &tp);
ERRCODE FileSetTime(const NHANDLE hFile, const time_point<system_clock> &tp);

// Returns the file size and modification time given the path or open file handle
ERRCODE FileGetInfo(PCNSTR szPath, uint64_t &nSize, time_point<system_clock> &tp);
ERRCODE FileGetInfo(const NHANDLE hFile, uint64_t &nSize, time_point<system_clock> &tp);

#ifdef _WIN32
ERRCODE FileGetAttributes(PCNSTR szPath, DWORD &dwAttrib);
ERRCODE FileGetAttributes(const NHANDLE hFile, DWORD &dwAttrib);

ERRCODE FileSetAttributes(PCNSTR szPath, const DWORD dwAttrib);
ERRCODE FileSetAttributes(const NHANDLE hFile, const DWORD dwAttrib);
#else
ERRCODE FileGetPermissions(PCNSTR szPath, mode_t &nMode);
ERRCODE FileGetPermissions(const NHANDLE hFile, mode_t &nMode);

ERRCODE FileSetPermissions(PCNSTR szPath, const mode_t nMode);
ERRCODE FileSetPermissions(const NHANDLE hFile, const mode_t nMode);
#endif

// Copies a file from the source to the destination, overwriting if it exists
ERRCODE FileCopy(PCNSTR szPathSrc, PCNSTR szPathDest);
// Moves a file from the source to the destination, overwriting if it exists
ERRCODE FileMove(PCNSTR szPathSrc, PCNSTR szPathDest);

NS_END
