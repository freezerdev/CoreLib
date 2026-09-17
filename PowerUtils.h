#pragma once

NS_BEGIN

enum class EPowerSource : uint8_t
{
	Unknown,
	AC,			// AC/mains power
	Battery		// Battery or UPS power
};

EPowerSource GetCurrentPowerSource(void);

bool IsBatterySaverActive(void);

#ifdef _WIN32
bool HasActiveDisplay(void);
#elif __APPLE__
enum class ELidState : uint8_t
{
	Unknown,
	Closed,
	Open
};

ELidState GetLidState(void);
#endif

NS_END
