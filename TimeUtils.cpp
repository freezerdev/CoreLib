#include "Base.h"
#include "TimeUtils.h"
#include <cmath>
#include <ctime>

NS_BEGIN

static constexpr auto POSIX_EPOCH = 25569.0;

//#################################################################################################
time_point<system_clock> GetCurrentUtcTime(void)
{
	return system_clock::now();
}

//#################################################################################################
double GetCurrentLocalTimeAsDouble(void)
{
	double dTime;

	TimePointToDoubleTime(GetCurrentUtcTime(), dTime);
	UtcToLocal(dTime);

	return dTime;
}

//#################################################################################################
int GetTimeZoneOffset(void)
{
	int nOffset = 0;

#ifdef _WIN32
	FILETIME ftUtc = {0};
	FILETIME ftLocal = {0};
	GetSystemTimeAsFileTime(&ftUtc);
	FileTimeToLocalFileTime(&ftUtc, &ftLocal);
	int64_t nUtc = MAKEUINT64(ftUtc.dwHighDateTime, ftUtc.dwLowDateTime);
	int64_t nLocal = MAKEUINT64(ftLocal.dwHighDateTime, ftLocal.dwLowDateTime);
	nOffset = (int)((nLocal - nUtc) / 10000000LL);
#else
	std::time_t nTime = system_clock::to_time_t(GetCurrentUtcTime());
	std::tm tmLocal = {0};
	std::tm tmUtc = {0};

	localtime_r(&nTime, &tmLocal);
	gmtime_r(&nTime, &tmUtc);

	std::time_t nTimeLocal = std::mktime(&tmLocal);
	time_point<system_clock> tpLocal = system_clock::from_time_t(nTimeLocal);

	tmUtc.tm_isdst = -1;
	std::time_t nTimeUtc = std::mktime(&tmUtc);
	time_point<system_clock> tpUtc = system_clock::from_time_t(nTimeUtc);

	nOffset = (int)((tpLocal - tpUtc) / seconds(1));
#endif

	return nOffset;
}

//#################################################################################################
CStr GetFormattedTime(const time_point<system_clock> &tp, const bool bConvertToLocal, const NCHAR chDateSeparator, const NCHAR chTimeSeparator, const NCHAR chFieldSeparator)
{
	CStr strTime;

	std::time_t nTime = system_clock::to_time_t(tp);
	std::tm tmTime = {0};

#ifdef _WIN32
	if(bConvertToLocal)
		localtime_s(&tmTime, &nTime);
	else
		gmtime_s(&tmTime, &nTime);
#else
	if(bConvertToLocal)
		localtime_r(&nTime, &tmTime);
	else
		gmtime_r(&nTime, &tmTime);
#endif

	strTime.Printf(_N("%d%c%02d%c%02d%c%02d%c%02d%c%02d"), tmTime.tm_year + 1900, chDateSeparator, tmTime.tm_mon + 1, chDateSeparator, tmTime.tm_mday, chFieldSeparator, tmTime.tm_hour, chTimeSeparator, tmTime.tm_min, chTimeSeparator, tmTime.tm_sec);

	return strTime;
}

//#################################################################################################
CStr GetFormattedDate(const time_point<system_clock> &tp, const bool bConvertToLocal, const NCHAR chDateSeparator)
{
	CStr strDate;

	std::time_t nTime = system_clock::to_time_t(tp);
	std::tm tmTime = {0};

#ifdef _WIN32
	if(bConvertToLocal)
		localtime_s(&tmTime, &nTime);
	else
		gmtime_s(&tmTime, &nTime);
#else
	if(bConvertToLocal)
		localtime_r(&nTime, &tmTime);
	else
		gmtime_r(&nTime, &tmTime);
#endif

	if(chDateSeparator == 0)
		strDate.Printf(_N("%d%02d%02d"), tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday);
	else
		strDate.Printf(_N("%d%c%02d%c%02d"), tmTime.tm_year + 1900, chDateSeparator, tmTime.tm_mon + 1, chDateSeparator, tmTime.tm_mday);

	return strDate;
}

//#################################################################################################
void EpochToTimePoint(const uint64_t nEpoch, time_point<system_clock> &tp, const EEpochType eType)
{
	CStr strEpoch = IntegerToString(nEpoch);
	EpochToTimePoint(strEpoch, tp, eType);
}

//#################################################################################################
void EpochToTimePoint(PCNSTR szEpoch, time_point<system_clock> &tp, const EEpochType eType)
{
	Assert(IsDigitString(szEpoch));
	Assert(eType != EET_Nanoseconds);

	uint64_t nTime = StringToInteger<uint64_t>(szEpoch);
	size_t nLen = StringGetLength(szEpoch);

	if(nLen >= 15 || eType == EET_Microseconds)
		tp = time_point<system_clock>(microseconds{nTime});
	else if(nLen >= 11 || eType == EET_Milliseconds)
		tp = time_point<system_clock>(milliseconds{nTime});
	else
		tp = time_point<system_clock>(seconds{nTime});
}

//#################################################################################################
void TimePointToEpoch(const time_point<system_clock> &tp, uint64_t &nEpoch, const EEpochType eType)
{
	switch(eType)
	{
	case EET_Seconds:
		nEpoch = duration_cast<seconds>(tp.time_since_epoch()).count();
		break;

	case EET_Milliseconds:
		nEpoch = duration_cast<milliseconds>(tp.time_since_epoch()).count();
		break;

	case EET_Microseconds:
		nEpoch = duration_cast<microseconds>(tp.time_since_epoch()).count();
		break;

	case EET_Nanoseconds:
		nEpoch = duration_cast<nanoseconds>(tp.time_since_epoch()).count();
		break;

	default:
		Assert(false);
		nEpoch = 0;
		break;
	}
}

//#################################################################################################
void TimePointToFileTime(const time_point<system_clock> &tp, FILETIME &ft)
{
	auto n = (system_clock::to_time_t(tp) + 11644473600ULL) * 10000000ULL;
	n += (tp.time_since_epoch() / nanoseconds(100)) % 10000000ULL;
	ft.dwLowDateTime = LO_UINT32(n);
	ft.dwHighDateTime = HI_UINT32(n);
}

//#################################################################################################
void FileTimeToTimePoint(const FILETIME &ft, time_point<system_clock> &tp)
{
	ULARGE_INTEGER li;
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	tp = system_clock::from_time_t((li.QuadPart - 116444736000000000ULL) / 10000000ULL);
	tp += microseconds((li.QuadPart % 10000000ULL) / 10ULL);
}

//#################################################################################################
void DoubleTimeToFileTime(const double dTime, FILETIME &ft)
{
	time_point<system_clock> tp;
	DoubleTimeToTimePoint(dTime, tp);
	TimePointToFileTime(tp, ft);
}

//#################################################################################################
void FileTimeToDoubleTime(const FILETIME &ft, double &dTime)
{
	time_point<system_clock> tp;
	FileTimeToTimePoint(ft, tp);
	TimePointToDoubleTime(tp, dTime);
}

//#################################################################################################
void TimePointToDosDateTime(const time_point<system_clock> &tp, uint16_t &nDosDate, uint16_t &nDosTime)
{
	std::time_t nTime = system_clock::to_time_t(tp);
	std::tm tmTime = {0};
#ifdef _WIN32
	localtime_s(&tmTime, &nTime);
#else
	localtime_r(&nTime, &tmTime);
#endif

	int nYearBits = ((tmTime.tm_year - 80) << 9) & 0xFE00;
	int nMonthBits = ((tmTime.tm_mon + 1) << 5) & 0x1E0;
	int nDayBits = tmTime.tm_mday & 0x1F;
	nDosDate = (uint16_t)(nYearBits | nMonthBits | nDayBits);

	int nHourBits = (tmTime.tm_hour << 11) & 0xF800;
	int nMinuteBits = (tmTime.tm_min << 5) & 0x7E0;
	int nSecondBits = (tmTime.tm_sec / 2) & 0x1F;
	nDosTime = (uint16_t)(nHourBits | nMinuteBits | nSecondBits);
}

//#################################################################################################
void DosDateTimeToTimePoint(const uint16_t nDosDate, const uint16_t nDosTime, time_point<system_clock> &tp)
{
	std::tm tmTime = {0};
	tmTime.tm_isdst = -1;

	tmTime.tm_year = ((nDosDate & 0xFE00) >> 9) + 80;
	tmTime.tm_mon = ((nDosDate & 0x1E0) >> 5) - 1;
	tmTime.tm_mday = nDosDate & 0x1F;

	tmTime.tm_hour = (nDosTime & 0xF800) >> 11;
	tmTime.tm_min = (nDosTime & 0x7E0) >> 5;
	tmTime.tm_sec = (nDosTime & 0x1F) * 2;

	std::time_t nTime = mktime(&tmTime);
	tp = system_clock::from_time_t(nTime);
}

//#################################################################################################
void TimePointToDoubleTime(const time_point<system_clock> &tp, double &dTime)
{	// Converting from numbers to decimals may lose precision and result in a small microsecond difference
	auto nDays = tp.time_since_epoch() / hours(24);
	auto nSinceMidnight = (tp.time_since_epoch() - hours(nDays * 24)) / nanoseconds(1);

	dTime = POSIX_EPOCH;
	dTime += nDays;
	dTime += (double)nSinceMidnight / 86400000000000.0;
}

//#################################################################################################
void DoubleTimeToTimePoint(const double dTime, time_point<system_clock> &tp)
{	// Converting from decimals to numbers may lose precision and result in a small microsecond difference
	double dDays;
	double dSinceMidnight = std::modf(dTime - POSIX_EPOCH, &dDays);

	// Attempt to compensate for the loss of precision
	dSinceMidnight += 0.00000000001;
	if(FloatIsGreaterEqual(dSinceMidnight, 1.0))
		dSinceMidnight -= 0.00000000001;

	uint64_t nMicroseconds = (uint64_t)(dSinceMidnight * 86400000000.0);
	uint64_t nMilliseconds = nMicroseconds / 1000;
	nMicroseconds = nMicroseconds % 1000;
	uint64_t nSeconds = nMilliseconds / 1000;
	nMilliseconds = nMilliseconds % 1000;
	uint64_t nMinutes = nSeconds / 60;
	nSeconds = nSeconds % 60;
	uint64_t nHours = nMinutes / 60;
	nMinutes = nMinutes % 60;

	tp = time_point<system_clock>();
	tp += hours((size_t)(((uint64_t)dDays * 24) + nHours));
	tp += minutes((size_t)nMinutes);
	tp += seconds((size_t)nSeconds);
	tp += milliseconds((size_t)nMilliseconds);
	tp += microseconds((size_t)nMicroseconds);
}

//#################################################################################################
void UtcToLocal(const time_point<system_clock> &tpUtc, time_point<system_clock> &tpLocal)
{
	tpLocal = (tpUtc.time_since_epoch().count() == 0) ? tpUtc : tpUtc + seconds(GetTimeZoneOffset());
}

//#################################################################################################
void UtcToLocal(time_point<system_clock> &tp)
{
	if(tp.time_since_epoch().count())
		tp += seconds(GetTimeZoneOffset());
}

//#################################################################################################
void LocalToUtc(const time_point<system_clock> &tpLocal, time_point<system_clock> &tpUtc)
{
	tpUtc = (tpLocal.time_since_epoch().count() == 0) ? tpLocal : tpLocal - seconds(GetTimeZoneOffset());
}

//#################################################################################################
void LocalToUtc(time_point<system_clock> &tp)
{
	if(tp.time_since_epoch().count())
		tp -= seconds(GetTimeZoneOffset());
}

//#################################################################################################
void UtcToLocal(const double dUtc, double &dLocal)
{
	dLocal = (FloatIsZero(dUtc)) ? dUtc : dUtc + ((double)GetTimeZoneOffset() / 86400.0);
}

//#################################################################################################
void UtcToLocal(double &dTime)
{
	if(!FloatIsZero(dTime))
		dTime += (double)GetTimeZoneOffset() / 86400.0;
}

//#################################################################################################
void LocalToUtc(const double dLocal, double &dUtc)
{
	dUtc = (FloatIsZero(dLocal)) ? dLocal : dLocal - ((double)GetTimeZoneOffset() / 86400.0);
}

//#################################################################################################
void LocalToUtc(double &dTime)
{
	if(!FloatIsZero(dTime))
		dTime -= (double)GetTimeZoneOffset() / 86400.0;
}

//#################################################################################################
time_point<system_clock> CreateTime(const uint16_t nYear, const uint8_t nMonth, const uint8_t nDay, const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond)
{
	Assert(nMonth != 0 && nMonth <= 12);
	Assert(nDay != 0 && nDay <= 31);
	Assert(nHour < 24);
	Assert(nMinute < 60);
	Assert(nSecond < 60);

	time_point<system_clock> tp;

	std::tm tmTime = {0};
	tmTime.tm_isdst = -1;

	tmTime.tm_year = nYear - 1900;
	tmTime.tm_mon = nMonth - 1;
	tmTime.tm_mday = nDay;

	tmTime.tm_hour = nHour;
	tmTime.tm_min = nMinute;
	tmTime.tm_sec = nSecond;

	std::time_t nTime = std::mktime(&tmTime);
	tp = system_clock::from_time_t(nTime);

	return tp;
}

//#################################################################################################
void SplitTime(const time_point<system_clock> &tp, uint16_t *pnYear, uint8_t *pnMonth, uint8_t *pnDay, uint8_t *pnHour, uint8_t *pnMinute, uint8_t *pnSecond, const bool bConvertToLocal)
{
	std::time_t nTime = system_clock::to_time_t(tp);
	std::tm tmTime = {0};

#ifdef _WIN32
	if(bConvertToLocal)
		localtime_s(&tmTime, &nTime);
	else
		gmtime_s(&tmTime, &nTime);
#else
	if(bConvertToLocal)
		localtime_r(&nTime, &tmTime);
	else
		gmtime_r(&nTime, &tmTime);
#endif

	if(pnYear)
		*pnYear = (uint16_t)tmTime.tm_year + 1900;

	if(pnMonth)
		*pnMonth = (uint8_t)tmTime.tm_mon + 1;

	if(pnDay)
		*pnDay = (uint8_t)tmTime.tm_mday;

	if(pnHour)
		*pnHour = (uint8_t)tmTime.tm_hour;

	if(pnMinute)
		*pnMinute = (uint8_t)tmTime.tm_min;

	if(pnSecond)
		*pnSecond = (uint8_t)tmTime.tm_sec;
}

//#################################################################################################
uint8_t GetDayOfTheWeek(const time_point<system_clock> &tp, const bool bConvertToLocal)
{
	std::time_t nTime = system_clock::to_time_t(tp);
	std::tm tmTime = {0};

#ifdef _WIN32
	if(bConvertToLocal)
		localtime_s(&tmTime, &nTime);
	else
		gmtime_s(&tmTime, &nTime);
#else
	if(bConvertToLocal)
		localtime_r(&nTime, &tmTime);
	else
		gmtime_r(&nTime, &tmTime);
#endif

	return (uint8_t)tmTime.tm_wday;
}

NS_END
