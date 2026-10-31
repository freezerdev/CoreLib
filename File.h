#pragma once

NS_BEGIN

//#################################################################################################
class CFile final
{
public:
	enum class EFileMode : uint8_t
	{
		ExistingReadOnly,		// Open existing file for read only
		ExistingReadWrite,		// Open existing file for read/write
		AlwaysReadWrite,		// Open existing file (or create new if not existing) for read/write
		CreateWriteOnly,		// Create new file for write only (overwrites existing)
		CreateReadWrite			// Create new file for read/write (overwrites existing)
	};

	enum class EFileSeek : uint8_t
	{
		Begin,					// Seek from the beginning of the file
		Current,				// Seek from the current position
		End						// Seek from the end of the file
	};

	CFile(void) noexcept;
	CFile(const CFile &src) = delete;
	CFile(CFile &&src) noexcept;
	~CFile(void);

	CFile &operator=(const CFile &src) = delete;
	CFile &operator=(CFile &&src) noexcept;

	CFile &Attach(NHANDLE hFile);
	NHANDLE Detach(void);

	bool IsOpen(void) const noexcept;
	operator NHANDLE(void) const noexcept;

	// Opens an existing file or creates a new file, returns FW_NO_ERROR on success or an error code
	ERRCODE Create(PCNSTR szPath, const EFileMode eMode);
	// Closes an open file
	ERRCODE Close(void);
#ifdef _WIN32
	// Marks (or unmarks) a file for deletion upon close
	ERRCODE MarkForDeletion(const bool bDelete = true) const;
#endif

	// Reads 'nBytesToRead' of data from the open file and copies the data to 'pBuffer', the number of bytes read is returned in 'nBytesRead'
	ERRCODE Read(PVOID pBuffer, const size_t nBytesToRead, size_t &nBytesRead) const;
	// Writes 'nBytesToWrite' of data to the open file from 'pBuffer', the number of bytes written is returned in 'nBytesWritten'
	ERRCODE Write(PCVOID pBuffer, const size_t nBytesToWrite, size_t &nBytesWritten) const;
	// Flushes data to disk
	ERRCODE Flush(void) const;

	// Gets the current file position
	ERRCODE GetPosition(uint64_t &nOffset) const;
	// Sets the current file position relative to 'eSeek'
	ERRCODE SetPosition(const int64_t nOffset, const EFileSeek eSeek = EFileSeek::Current) const;
	// Sets the current file position as the end of the file (truncates any file after current position)
	ERRCODE SetEnd(void) const;

	// Returns the file size of the open file
	ERRCODE GetSize(uint64_t &nSize) const;
	// Returns the file modification time of the open file
	ERRCODE GetTime(time_point<system_clock> &tp) const;
	// Sets the file modification time of the open file
	ERRCODE SetTime(const time_point<system_clock> &tp) const;
	// Returns the file size and modification of the open file
	ERRCODE GetInfo(uint64_t &nSize, time_point<system_clock> &tp) const;

#ifdef _WIN32
	ERRCODE GetAttributes(DWORD &dwAttrib) const;
	ERRCODE SetAttributes(const DWORD dwAttrib) const;
#else
	ERRCODE GetPermissions(mode_t &nMode) const;
	ERRCODE SetPermissions(const mode_t nMode) const;
#endif

private:
	NHANDLE m_hFile;
};

NS_END
