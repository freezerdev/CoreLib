#pragma once

NS_BEGIN

enum EPowerSource : uint8_t
{
	EPS_Unknown = 0,
	EPS_AC,			// AC/mains power
	EPS_Battery		// Battery or UPS power
};

EPowerSource GetCurrentPowerSource(void);

bool IsBatterySaverActive(void);

#ifdef _WIN32
bool HasActiveDisplay(void);
#elif __APPLE__
enum ELidState : uint8_t
{
	ELS_Unknown = 0,
	ELS_Closed,
	ELS_Open
};

ELidState GetLidState(void);
#endif

NS_END
