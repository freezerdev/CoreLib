#pragma once

#ifdef _WIN32
#include <shlobj.h>
#endif

NS_BEGIN

#ifdef _WIN32
CFilePathW GetKnownFolder(const KNOWNFOLDERID &id);
// Returns the path to the Windows directory
CFilePathW GetWindowsFolder(void);
// Returns the path to the Windows system directory
CFilePathW GetSystemFolder(void);
// Returns the path to the current working directory for the given drive
CFilePathW GetCurrentFolder(const CFilePathSegmentW &drive);
#endif

// Returns the path to the system temporary directory
CFilePath GetTempFolder(void);
// Returns the path to the current working directory
CFilePath GetCurrentFolder(void);
// Set the current working directory to the path
bool SetCurrentFolder(PCNSTR szPath);
// Returns the full path the to executing process
CFilePath GetExecutablePath(void);

#ifdef _WIN32
// Converts a string/path to the long file name version
bool ConvertToLong(CStrW &str);
bool ConvertToLong(CFilePathW &path);
// Converts a string/path to the short file name (8.3) version
bool ConvertToShort(CStrW &str);
bool ConvertToShort(CFilePathW &path);

// Converts a UNC path to its most simple form if the UNC path is on the current computer, resolving UNC paths to get the root canonical file
bool CanonicalizeLocalUNC(CFilePathW &path);
#endif
// Converts a path to its most simple form, resolving links to get the root canonical file
bool CanonicalizeLink(CFilePath &path);
// Converts a relative path to a rooted path
bool CanonicalizeRelative(CFilePath &path);
// Converts a path to its most simple form, resolving links, relative paths, and UNC paths (for Windows) to get the root canonical file
bool Canonicalize(CFilePath &path);

NS_END
