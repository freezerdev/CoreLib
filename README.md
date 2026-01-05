CoreLib is a cross-platform C++ library with many useful functions and classes for non-GUI development.  This SDK supports Windows, MacOS, and Linux platforms.

The following files are in the SDK:

[Base](https://github.com/freezerdev/CoreLib/blob/main/Base.h) - Common defines, macros, and includes for the SDK.

[BlockingQueue](https://github.com/freezerdev/CoreLib/blob/main/BlockingQueue.h) - A thread-safe efficient queue.

[CityHash](https://github.com/freezerdev/CoreLib/blob/main/CityHash.cpp) - A clean implementation of Google's fast "CityHash" function.

[Container](https://github.com/freezerdev/CoreLib/blob/main/Container.cpp) - A container class for storing sequential data of varying types (strings, integers, floats, and raw buffers).

[CoreLib](https://github.com/freezerdev/CoreLib/blob/main/CoreLib.cpp) - Functions to initialize and free the entire CoreLib.

[CoreUtils](https://github.com/freezerdev/CoreLib/blob/main/CoreUtils.cpp) - Miscellaneous useful functions such as machine and username, command line arguments, UUID conversions, UU encoding, and proper comparison of floats/doubles.

[Crc32](https://github.com/freezerdev/CoreLib/blob/main/Crc32.cpp) - Classes to easily generate CRC32 checksums; includes both the ubiquitous PKzip algorithm as well as the MPEG2 algorithm.

[CritSec](https://github.com/freezerdev/CoreLib/blob/main/CritSec.cpp) - Wrapper class around a critical section (Windows only).

[Diagnostic](https://github.com/freezerdev/CoreLib/blob/main/Diagnostic.cpp) - Diagnostic assert messages to aid in debugging.

[DoubleNullStr](https://github.com/freezerdev/CoreLib/blob/main/DoubleNullStr.cpp) - A string class for double null-terminated strings.

[Errors](https://github.com/freezerdev/CoreLib/blob/main/Errors.cpp) - Common error codes so as to avoid platform-specific error codes.

[Event](https://github.com/freezerdev/CoreLib/blob/main/Event.cpp) - An event object for efficient multi-threaded synchronization, most useful for MacOS and Linux which lack an event kernel object as is found on Windows.

[ExceptionUtils](https://github.com/freezerdev/CoreLib/blob/main/ExceptionUtils.cpp) - Functions to create crash dump files to aid in postmortem crash analysis.

[FilePath](https://github.com/freezerdev/CoreLib/blob/main/FilePath.cpp) - An extremely useful class to build, parse, and analyze file and folder paths.

[FilePathConverter](https://github.com/freezerdev/CoreLib/blob/main/FilePathConverter.cpp) - A class to resolve path variables and kernel path names into standard paths (Windows only).

[FilePathUtils](https://github.com/freezerdev/CoreLib/blob/main/FilePathUtils.cpp) - Useful path functions to obtain common folder locations, as well as canonicalize (simplify) paths.

[FileSystemUtils](https://github.com/freezerdev/CoreLib/blob/main/FileSystemUtils.cpp) - Functions to create, enumerate, delete, and obtain info about file system objects.

[FileTraverse](https://github.com/freezerdev/CoreLib/blob/main/FileTraverse.cpp) - A class to efficiently traverse one or more folders, useful when enumerating file system objects or searching for files/folders.

[Fuzz](https://github.com/freezerdev/CoreLib/blob/main/Fuzz.h) - Functions to facilitate fuzz testing.

[Impersonator](https://github.com/freezerdev/CoreLib/blob/main/Impersonator.cpp) - A class to impersonate another logged on user given their session number (Windows only).  Can only be called from a service or process running in session 0.

[IniFile](https://github.com/freezerdev/CoreLib/blob/main/IniFile.cpp) - A class to read, edit, or create Windows INI files.

[IPv6](https://github.com/freezerdev/CoreLib/blob/main/IPv6.cpp) - A utility class for IPv6 addresses, useful to easily convert between binary and string representations.

[Mac](https://github.com/freezerdev/CoreLib/blob/main/Mac.cpp) - A utility class for network MAC addresses, useful to easily convert between binary and string representations.

[MemBuffer](https://github.com/freezerdev/CoreLib/blob/main/MemBuffer.cpp) - A wrapper class around a raw block of memory, useful for storing one or more strings, integers, floats, and raw buffers.

[MemoryHelper](https://github.com/freezerdev/CoreLib/blob/main/MemoryHelper.cpp) - A class that calls an undocumented function to obtain a process' private working set size (Windows only).

[MemStream](https://github.com/freezerdev/CoreLib/blob/main/MemStream.cpp) - A wrapper class around a raw block of memory similar to MemBuffer, but designed to be easily streamed.

[NetUtils](https://github.com/freezerdev/CoreLib/blob/main/NetUtils.cpp) - Useful network utilities including IPv4 conversions, downloading files, obtaining the public IP address, and more.

[Object](https://github.com/freezerdev/CoreLib/blob/main/Object.cpp) - A wrapper class for MacOS CoreFountation object, to ensure proper release (MacOS only).

[PlatformUtils](https://github.com/freezerdev/CoreLib/blob/main/PlatformUtils.cpp) - Some platform-specific utilities including COM and SIDs on Windows as well as IO registry and Sysctl on MacOS.

[PowerUtils](https://github.com/freezerdev/CoreLib/blob/main/PowerUtils.cpp) - Utilities that gather power related info such as battery versus mains power and laptop lid state (opened/closed).

[PrivilegeManager](https://github.com/freezerdev/CoreLib/blob/main/PrivilegeManager.cpp) - A class to manage granting/revoking of process privileges such as "SE_TCB_NAME" (Windows only).

[ProcessUtils](https://github.com/freezerdev/CoreLib/blob/main/ProcessUtils.cpp) - Process utilities such as spawning a new process and obtaining any process' filename or command line.

[RandUtils](https://github.com/freezerdev/CoreLib/blob/main/RandUtils.cpp) - Wrapper class around the C++ random library functions.  Easily generate non-cryptographically-secure random numbers.

[RegexUtils](https://github.com/freezerdev/CoreLib/blob/main/RegexUtils.cpp) - Wrapper class around the C++ regex library functions.  More easily perform regex comparisons and matches.

[RegKey](https://github.com/freezerdev/CoreLib/blob/main/RegKey.cpp) - A class for reading, writing, creating, and deleting Windows registry keys (Windows only).

[RegPath](https://github.com/freezerdev/CoreLib/blob/main/RegPath.cpp) - An extremely useful class to build, parse, and analyze registry paths (Windows only).

[SrwLock](https://github.com/freezerdev/CoreLib/blob/main/SrwLock.cpp) - A wrapper class for slim reader/writer (SRW) locks (Windows only).

[StlHelper](https://github.com/freezerdev/CoreLib/blob/main/StlHelper.cpp) - Helper classes for using CoreLib objects with C++ STL 

[Str](https://github.com/freezerdev/CoreLib/blob/main/Str.cpp) - An extremely useful string class.  Has very useful capabilities such as formatting without printf; case-insensitive comparison; and automatic conversion between UTF8, wide characters, and UTF16.

[StrUtils](https://github.com/freezerdev/CoreLib/blob/main/StrUtils.cpp) - Standalone string functions useful when manipulating null-terminated C strings.

[TextFile](https://github.com/freezerdev/CoreLib/blob/main/TextFile.cpp) - A class for reading, creating, and modifying text files (UTF8, UTF16, and wide characters).

[Thread](https://github.com/freezerdev/CoreLib/blob/main/Thread.cpp) - A wrapper class for cross-platform thread use, including automatic converting unhandled exceptions into crash dump files for analysis.

[ThreadUtils](https://github.com/freezerdev/CoreLib/blob/main/ThreadUtils.cpp) - A function to obtain a thread ID (TID) for a given C++ thread object.

[TimeUtils](https://github.com/freezerdev/CoreLib/blob/main/TimeUtils.cpp) - Functions to convert between many different common time formats as well as UTC <-> local time conversions.

[Url](https://github.com/freezerdev/CoreLib/blob/main/Url.cpp) - An very useful class to build, parse, and analyze URLs.

[UrlUtils](https://github.com/freezerdev/CoreLib/blob/main/UrlUtils.cpp) - Functions to encode and decode URL strings.

[WhitespaceParser](https://github.com/freezerdev/CoreLib/blob/main/WhitespaceParser.cpp) - A class to parse strings into tokens using whitespace as the delimiter.

[Wmi](https://github.com/freezerdev/CoreLib/blob/main/Wmi.cpp) - A class for interacting with WMI objects (Windows only).

[WtsUtils](https://github.com/freezerdev/CoreLib/blob/main/WtsUtils.cpp) - Wrapper functions for some common Windows Terminal Services APIs (Windows only).

