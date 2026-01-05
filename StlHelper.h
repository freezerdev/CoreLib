#pragma once

NS_BEGIN

//#################################################################################################
template<typename T>
class CNumberHashTraits final
{
public:
	T operator()(const T n) const noexcept
	{
		return n;
	}
};

//#################################################################################################
class CStr8HashTraits final
{
public:
	uint64_t operator()(const CStr8 &str) const;
};

//#################################################################################################
class CStrWHashTraits final
{
public:
	uint64_t operator()(const CStrW &str) const;
};

//#################################################################################################
class CStr8IgnoreCaseTraits final
{
public:
	uint64_t operator()(const CStr8 &str) const;
	bool operator()(const CStr8 &str1, const CStr8 &str2) const;
};

//#################################################################################################
class CStrWIgnoreCaseTraits final
{
public:
	uint64_t operator()(const CStrW &str) const;
	bool operator()(const CStrW &str1, const CStrW &str2) const;
};

//#################################################################################################
class CFilePathSegment8HashTraits final
{
public:
	uint64_t operator()(const CFilePathSegment8 &path) const;
};

//#################################################################################################
class CFilePathSegmentWHashTraits final
{
public:
	uint64_t operator()(const CFilePathSegmentW &path) const;
};

//#################################################################################################
class CFilePath8HashTraits final
{
public:
	uint64_t operator()(const CFilePath8 &path) const;
};

//#################################################################################################
class CFilePathWHashTraits final
{
public:
	uint64_t operator()(const CFilePathW &path) const;
};

#ifdef _WIN32
//#################################################################################################
class CUuidHashTraits final
{	// You cannot use uuid_t in STL containers on MacOS because it is defined as a raw character array which is not copyable
public:
	uint64_t operator()(const uuid_t &id) const;
};

using CStrHashTraits = CStrWHashTraits;
using CStrIgnoreCaseTraits = CStrWIgnoreCaseTraits;
using CFilePathSegmentHashTraits = CFilePathSegmentWHashTraits;
using CFilePathHashTraits = CFilePathWHashTraits;
#else
using CStrHashTraits = CStr8HashTraits;
using CStrIgnoreCaseTraits = CStr8IgnoreCaseTraits;
using CFilePathSegmentHashTraits = CFilePathSegment8HashTraits;
using CFilePathHashTraits = CFilePath8HashTraits;
#endif

NS_END
