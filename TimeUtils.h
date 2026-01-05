#pragma once

NS_BEGIN

// Returns a time_point object of the current time in UTC
time_point<system_clock> GetCurrentUtcTime(void);

// Returns a double of the current local time
double GetCurrentLocalTimeAsDouble(void);

// Returns current time zone difference from UTC in total seconds
int GetTimeZoneOffset(void);

// Formats a time string, optionally converting from UTC to local time
CStr GetFormattedTime(const time_point<system_clock> &tp = GetCurrentUtcTime(), const bool bConvertToLocal = true, const NCHAR chDateSeparator = _N('-'), const NCHAR chTimeSeparator = _N(':'), const NCHAR chFieldSeparator = _N(' '));
CStr GetFormattedDate(const time_point<system_clock> &tp = GetCurrentUtcTime(), const bool bConvertToLocal = true, const NCHAR chDateSeparator = _N('-'));

enum EEpochType : uint8_t
{
	EET_Auto = 0,		// Automatically detect most likely epoch time format
	EET_Seconds,
	EET_Milliseconds,
	EET_Microseconds,
	EET_Nanoseconds
};

// Converts an epoch time (in seconds, milliseconds, or microseconds) to a time_point object
void EpochToTimePoint(const uint64_t nEpoch, time_point<system_clock> &tp, const EEpochType eType = EET_Auto);
void EpochToTimePoint(PCNSTR szEpoch, time_point<system_clock> &tp, const EEpochType eType = EET_Auto);

// Converts a time_point object to epoch time (in seconds, milliseconds, or microseconds)
void TimePointToEpoch(const time_point<system_clock> &tp, uint64_t &nEpoch, const EEpochType eType = EET_Auto);

// Converts a time_point object to/from a FILETIME
void TimePointToFileTime(const time_point<system_clock> &tp, FILETIME &ft);
void FileTimeToTimePoint(const FILETIME &ft, time_point<system_clock> &tp);

// Converts a variant (double) time to/from a FILETIME
void DoubleTimeToFileTime(const double dTime, FILETIME &ft);
void FileTimeToDoubleTime(const FILETIME &ft, double &dTime);

// Converts a time_point object to/from a DOS time
void TimePointToDosDateTime(const time_point<system_clock> &tp, uint16_t &nDosDate, uint16_t &nDosTime);
void DosDateTimeToTimePoint(const uint16_t nDosDate, const uint16_t nDosTime, time_point<system_clock> &tp);

// Converts a time_point object to/from a variant (double) time
void TimePointToDoubleTime(const time_point<system_clock> &tp, double &dTime);
void DoubleTimeToTimePoint(const double dTime, time_point<system_clock> &tp);

// Converts a time_point object between UTC and local time
void UtcToLocal(const time_point<system_clock> &tpUtc, time_point<system_clock> &tpLocal);
void UtcToLocal(time_point<system_clock> &tp);
void LocalToUtc(const time_point<system_clock> &tpLocal, time_point<system_clock> &tpUtc);
void LocalToUtc(time_point<system_clock> &tp);

// Converts a variant time between UTC and local time
void UtcToLocal(const double dUtc, double &dLocal);
void UtcToLocal(double &dTime);
void LocalToUtc(const double dLocal, double &dUtc);
void LocalToUtc(double &dTime);

// Creates a time_point object from individual elements, input times are local and output is in UTC
time_point<system_clock> CreateTime(const uint16_t nYear, const uint8_t nMonth, const uint8_t nDay, const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond);

// Splits a time_point object into individual elements, input time is in UTC
void SplitTime(const time_point<system_clock> &tp, uint16_t *pnYear = nullptr, uint8_t *pnMonth = nullptr, uint8_t *pnDay = nullptr, uint8_t *pnHour = nullptr, uint8_t *pnMinute = nullptr, uint8_t *pnSecond = nullptr, const bool bConvertToLocal = true);

// Returns the day or the week; Sunday=0, Monday=1, Tuesday=2, Wednesday=3, Thursday=4, Friday=5, Saturday=6
uint8_t GetDayOfTheWeek(const time_point<system_clock> &tp = GetCurrentUtcTime(), const bool bConvertToLocal = true);

NS_END
