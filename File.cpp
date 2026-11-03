#include "Base.h"
#include "File.h"
#include "TimeUtils.h"
#ifdef _WIN32
#include <versionhelpers.h>
#endif

NS_BEGIN

#ifdef _WIN32
constexpr auto ATTRIB_MASK = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_OFFLINE | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
#endif

//#################################################################################################
CFile::CFile(CFile &&src) noexcept
	: m_hFile(src.m_hFile)
{
	src.m_hFile = INVALID_NHANDLE;
}

//#################################################################################################
CFile::~CFile(void)
{
	Close();
}

//#################################################################################################
CFile &CFile::operator=(CFile &&src) noexcept
{
	if(this != &src)
	{
		m_hFile = src.m_hFile;
		src.m_hFile = INVALID_NHANDLE;
	}

	return *this;
}

//#################################################################################################
CFile &CFile::Attach(NHANDLE hFile)
{
	Close();

	m_hFile = hFile;

	return *this;
}

//#################################################################################################
NHANDLE CFile::Detach(void)
{	// Release the file, it is up to the caller to close the handle
	NHANDLE hFile = m_hFile;
	m_hFile = INVALID_NHANDLE;

	return hFile;
}

//#################################################################################################
bool CFile::IsOpen(void) const noexcept
{
	return (m_hFile != INVALID_NHANDLE);
}

//#################################################################################################
CFile::operator NHANDLE(void) const noexcept
{
	return m_hFile;
}

//#################################################################################################
ERRCODE CFile::Create(PCNSTR szPath, const EFileMode eMode)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Close();

	if(szPath == nullptr || *szPath == _N('\0'))
		return FW_ERROR_INVALID_PARAMETER;

#ifdef _WIN32
	DWORD dwAccess = 0;
	DWORD dwShare = FILE_SHARE_READ;
	DWORD dwDisposition = 0;
	switch(eMode)
	{
	case EFileMode::ExistingReadOnly:
		dwAccess = GENERIC_READ;
		ADD_FLAG(dwShare, FILE_SHARE_WRITE | FILE_SHARE_DELETE);
		dwDisposition = OPEN_EXISTING;
		break;

	case EFileMode::ExistingReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwDisposition = OPEN_EXISTING;
		break;

	case EFileMode::AlwaysReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwDisposition = OPEN_ALWAYS;
		break;

	case EFileMode::CreateWriteOnly:
		dwAccess = GENERIC_WRITE;
		ADD_FLAG(dwShare, FILE_SHARE_WRITE | FILE_SHARE_DELETE);
		dwDisposition = CREATE_ALWAYS;
		break;

	case EFileMode::CreateReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwDisposition = CREATE_ALWAYS;
		break;
	}

	m_hFile = CreateFileW(szPath, dwAccess, dwShare, nullptr, dwDisposition, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
		m_hFile = INVALID_NHANDLE;
	}
#else
	int nFlags = 0;
	switch(eMode)
	{
	case EFileMode::ExistingReadOnly:
		nFlags = O_RDONLY;
		break;

	case EFileMode::ExistingReadWrite:
		nFlags = O_RDWR;
		break;

	case EFileMode::AlwaysReadWrite:
		nFlags = O_RDWR | O_CREAT;
		break;

	case EFileMode::CreateWriteOnly:
		nFlags = O_WRONLY | O_CREAT | O_TRUNC;
		break;

	case EFileMode::CreateReadWrite:
		nFlags = O_RDWR | O_CREAT | O_TRUNC;
		break;
	}

	m_hFile = open(szPath, nFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(m_hFile == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::Close(void)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

#ifdef _WIN32
	if(m_hFile != INVALID_NHANDLE && !CloseHandle(m_hFile))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(m_hFile != INVALID_NHANDLE && close(m_hFile) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif
	m_hFile = INVALID_NHANDLE;

	return nErrorCode;
}

#ifdef _WIN32
//#################################################################################################
ERRCODE CFile::MarkForDeletion(const bool bDelete) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	FILE_DISPOSITION_INFO fdi;
	fdi.DeleteFile = bDelete;
	if(!SetFileInformationByHandle(m_hFile, FileDispositionInfo, &fdi, sizeof(fdi)))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}
#endif

//#################################################################################################
ERRCODE CFile::Read(PVOID pBuffer, const size_t nBytesToRead, size_t &nBytesRead) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nBytesRead = 0;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	DWORD dwBytesToRead = (DWORD)nBytesToRead;
	DWORD dwBytesRead;
	if(ReadFile(m_hFile, pBuffer, dwBytesToRead, &dwBytesRead, nullptr))
	{
		nBytesRead = (size_t)dwBytesRead;
		if(nBytesRead == 0)
			nErrorCode = FW_ERROR_HANDLE_EOF;
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	ssize_t nResult = read(m_hFile, pBuffer, nBytesToRead);
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
ERRCODE CFile::Write(PCVOID pBuffer, const size_t nBytesToWrite, size_t &nBytesWritten) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nBytesWritten = 0;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	DWORD dwBytesToWrite = (DWORD)nBytesToWrite;
	DWORD dwBytesWritten;
	if(WriteFile(m_hFile, pBuffer, dwBytesToWrite, &dwBytesWritten, nullptr))
		nBytesWritten = (size_t)dwBytesWritten;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	ssize_t nResult = write(m_hFile, pBuffer, nBytesToWrite);
	if(nResult >= 0)
		nBytesWritten = (size_t)nResult;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::Flush(void) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	if(!FlushFileBuffers(m_hFile))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	if(fsync(m_hFile) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::GetPosition(uint64_t &nOffset) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nOffset = 0;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	LARGE_INTEGER liMove = {0};
	LARGE_INTEGER liCurrent;
	if(SetFilePointerEx(m_hFile, liMove, &liCurrent, FILE_CURRENT))
		nOffset = liCurrent.QuadPart;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	off_t nCurrent = lseek(m_hFile, 0, SEEK_CUR);
	if(nCurrent != -1)
		nOffset = (uint64_t)nCurrent;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::SetPosition(const int64_t nOffset, EFileSeek eSeek) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	LARGE_INTEGER liMove;
	liMove.QuadPart = nOffset;

	int nMode = FILE_BEGIN;
	switch(eSeek)
	{
	case EFileSeek::Begin:
		nMode = FILE_BEGIN;
		break;

	case EFileSeek::Current:
		nMode = FILE_CURRENT;
		break;

	case EFileSeek::End:
		nMode = FILE_END;
		break;
	}

	if(!SetFilePointerEx(m_hFile, liMove, nullptr, nMode))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	int nSeek;
	switch(eSeek)
	{
	case EFileSeek::Begin:
		nSeek = SEEK_SET;
		break;

	case EFileSeek::Current:
		nSeek = SEEK_CUR;
		break;

	case EFileSeek::End:
		nSeek = SEEK_END;
		break;
	}

	if(lseek(m_hFile, nOffset, nSeek) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::SetEnd(void) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	if(!SetEndOfFile(m_hFile))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	off_t nOffset = lseek(m_hFile, 0, SEEK_CUR);
	if(nOffset != -1)
	{
		if(ftruncate(m_hFile, nOffset) == -1)
			nErrorCode = ConvertFromNativeErrorCode(errno);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::GetSize(uint64_t &nSize) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nSize = 0;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	LARGE_INTEGER li;
	if(GetFileSizeEx(m_hFile, &li))
		nSize = li.QuadPart;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	off_t nCurrent = lseek(m_hFile, 0, SEEK_CUR);
	if(nCurrent != -1)
	{
		off_t nEnd = lseek(m_hFile, 0, SEEK_END);
		if(nEnd != -1)
		{
			nSize = (uint64_t)nEnd;

			if(lseek(m_hFile, nCurrent, SEEK_SET) == -1)
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
ERRCODE CFile::GetTime(time_point<system_clock> &tp) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	tp = time_point<system_clock>();

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	FILE_BASIC_INFO fbi = {0};
	if(GetFileInformationByHandleEx(m_hFile, FileBasicInfo, &fbi, sizeof(fbi)))
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
	if(fstat(m_hFile, &s) == 0)
		tp = system_clock::from_time_t(s.st_mtime);
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::SetTime(const time_point<system_clock> &tp) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	FILETIME ft;
	TimePointToFileTime(tp, ft);
	if(!SetFileTime(m_hFile, nullptr, nullptr, &ft))
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#elif __APPLE__
	auto ns = tp.time_since_epoch() - seconds(tp.time_since_epoch() / seconds(1));

	timeval times[2];
	times[0].tv_sec = times[1].tv_sec = system_clock::to_time_t(tp);
	times[0].tv_usec = times[1].tv_usec = (int)(ns.count() / 1000);
	if(futimes(m_hFile, times) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#elif __linux__
	auto ns = tp.time_since_epoch() - seconds(tp.time_since_epoch() / seconds(1));

	struct timespec times[2];
	times[0].tv_sec = times[1].tv_sec = system_clock::to_time_t(tp);
	times[0].tv_nsec = times[1].tv_nsec = ns.count();
	if(futimens(m_hFile, times) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
#endif

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::GetInfo(uint64_t &nSize, time_point<system_clock> &tp) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nSize = 0;
	tp = time_point<system_clock>();

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

#ifdef _WIN32
	BY_HANDLE_FILE_INFORMATION bhfi = {0};
	if(GetFileInformationByHandle(m_hFile, &bhfi))
	{
		nSize = MAKEUINT64(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
		FileTimeToTimePoint(bhfi.ftLastWriteTime, tp);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());
#else
	struct stat s;
	if(fstat(m_hFile, &s) == 0)
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
ERRCODE CFile::GetAttributes(DWORD &dwAttrib) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	dwAttrib = 0;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	FILE_BASIC_INFO fbi = {0};
	if(GetFileInformationByHandleEx(m_hFile, FileBasicInfo, &fbi, sizeof(fbi)))
		dwAttrib = fbi.FileAttributes;
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::SetAttributes(const DWORD dwAttrib) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert((dwAttrib & ~ATTRIB_MASK) == 0);

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	FILE_BASIC_INFO fbi = {0};
	if(GetFileInformationByHandleEx(m_hFile, FileBasicInfo, &fbi, sizeof(fbi)))
	{
		REMOVE_FLAG(fbi.FileAttributes, ATTRIB_MASK);
		ADD_FLAG(fbi.FileAttributes, dwAttrib);
		if(!SetFileInformationByHandle(m_hFile, FileBasicInfo, &fbi, sizeof(fbi)))
			nErrorCode = ConvertFromNativeErrorCode(GetLastError());
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

#else

//#################################################################################################
ERRCODE CFile::GetPermissions(mode_t &nMode) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	nMode = 0;

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	struct stat s;
	if(fstat(m_hFile, &s) == 0)
		nMode = s.st_mode & ~S_IFMT;
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFile::SetPermissions(const mode_t nMode) const
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Assert(!HAS_FLAG(nMode, S_IFMT));

	if(m_hFile == INVALID_NHANDLE)
		return FW_ERROR_INVALID_HANDLE;

	if(fchmod(m_hFile, nMode) == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}
#endif

NS_END
