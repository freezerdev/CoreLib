#include "Base.h"
#include "StlHelper.h"
#include "CityHash.h"

NS_BEGIN

//#################################################################################################
uint64_t CStr8HashTraits::operator()(const CStr8 &str) const
{
	return CityHash64((PCBYTE)(PCSTR)str, str.GetSize());
}

//#################################################################################################
uint64_t CStrWHashTraits::operator()(const CStrW &str) const
{
	return CityHash64((PCBYTE)(PCWSTR)str, str.GetSize());
}

//#################################################################################################
uint64_t CStr8IgnoreCaseTraits::operator()(const CStr8 &str) const
{
	CStr8 strLower(str);
	strLower.MakeLower();
	return CityHash64((PCBYTE)(PCSTR)strLower, strLower.GetSize());
}

//#################################################################################################
bool CStr8IgnoreCaseTraits::operator()(const CStr8 &str1, const CStr8 &str2) const
{
	return (str1.Compare(str2, true) == 0);
}

//#################################################################################################
uint64_t CStrWIgnoreCaseTraits::operator()(const CStrW &str) const
{
	CStrW strLower(str);
	strLower.MakeLower();
	return CityHash64((PCBYTE)(PCWSTR)strLower, strLower.GetSize());
}

//#################################################################################################
bool CStrWIgnoreCaseTraits::operator()(const CStrW &str1, const CStrW &str2) const
{
	return (str1.Compare(str2, true) == 0);
}

//#################################################################################################
uint64_t CFilePathSegment8HashTraits::operator()(const CFilePathSegment8 &path) const
{
	CStr8 strPath(path);
	strPath.MakeLower();
	return CityHash64((PCBYTE)(PCSTR)strPath, strPath.GetSize());
}

//#################################################################################################
uint64_t CFilePathSegmentWHashTraits::operator()(const CFilePathSegmentW &path) const
{
	CStrW strPath(path);
	strPath.MakeLower();
	return CityHash64((PCBYTE)(PCWSTR)strPath, strPath.GetSize());
}

//#################################################################################################
uint64_t CFilePath8HashTraits::operator()(const CFilePath8 &path) const
{
	CStr8 strPath(path);
	strPath.MakeLower();
	return CityHash64((PCBYTE)(PCSTR)strPath, strPath.GetSize());
}

//#################################################################################################
uint64_t CFilePathWHashTraits::operator()(const CFilePathW &path) const
{
	CStrW strPath(path);
	strPath.MakeLower();
	return CityHash64((PCBYTE)(PCWSTR)strPath, strPath.GetSize());
}

#ifdef _WIN32
//#################################################################################################
uint64_t CUuidHashTraits::operator()(const uuid_t &id) const
{
	CStr strId = UuidToString(id);
	return CityHash64((PCBYTE)(PCNSTR)strId, strId.GetSize());
}
#endif

NS_END
