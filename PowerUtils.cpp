#include "Base.h"
#include "PowerUtils.h"
#ifdef _WIN32
#include "PlatformUtils.h"
#include <dxgi.h>
#elif __APPLE__
#include "Object.h"
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/pwr_mgt/IOPM.h>
#endif

NS_BEGIN

//#################################################################################################
EPowerSource GetCurrentPowerSource(void)
{
	EPowerSource eSource = EPS_Unknown;

#ifdef _WIN32
	SYSTEM_POWER_STATUS sps = {0};
	GetSystemPowerStatus(&sps);
	if(sps.ACLineStatus == AC_LINE_ONLINE)
		eSource = EPS_AC;
	else if(sps.ACLineStatus != AC_LINE_UNKNOWN)
		eSource = EPS_Battery;
#elif __APPLE__
	CCFObject cfPower = IOPSCopyPowerSourcesInfo();
	if(cfPower)
	{
		CCFObject cfValue(IOPSGetProvidingPowerSourceType(cfPower), false);
		if(cfValue)
		{
			CStr strSource = cfValue.GetString();

			if(strSource == kIOPMACPowerKey)
				eSource = EPS_AC;
			else if(strSource == kIOPMBatteryPowerKey || strSource == kIOPMUPSPowerKey)
				eSource = EPS_Battery;
		}
	}
#elif __linux__
	// TODO
#endif

	return eSource;
}

//#################################################################################################
bool IsBatterySaverActive(void)
{
	bool bBatterySaverActive = false;

#ifdef _WIN32
	SYSTEM_POWER_STATUS sps = {0};
	GetSystemPowerStatus(&sps);
	if(sps.SystemStatusFlag)
		bBatterySaverActive = true;
#elif __APPLE__
	IOPSLowBatteryWarningLevel eLevel = IOPSGetBatteryWarningLevel();
	if(eLevel == kIOPSLowBatteryWarningEarly || eLevel == kIOPSLowBatteryWarningFinal)
		bBatterySaverActive = true;
#elif __linux__
	// TODO
#endif

	return bBatterySaverActive;
}

#ifdef _WIN32
//#################################################################################################
bool HasActiveDisplay(void)
{	// Just because the laptop lid is closed does not mean the user is not interactive, there could be an external display
#ifdef _DEBUG
	DWORD dwSessionId;
	if(ProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId))
		Assert(dwSessionId);	// This function does not work when called from a service running in session 0
#endif

	bool bHasActiveDisplay = false;

	using PFNCREATEDXGIFACTORY = HRESULT(WINAPI*)(const IID&, PVOID*);

	HMODULE hDxgi = SystemLoadLibrary(_N("dxgi.dll"));
	if(hDxgi)
	{
		PFNCREATEDXGIFACTORY pfnCreateDXGIFactory = (PFNCREATEDXGIFACTORY)GetProcAddress(hDxgi, "CreateDXGIFactory");
		if(pfnCreateDXGIFactory)
		{
			IDXGIFactory *pFactory = nullptr;
			if(SUCCEEDED(pfnCreateDXGIFactory(__uuidof(IDXGIFactory), (PVOID*)&pFactory)))
			{
				DXGI_OUTPUT_DESC od;
				IDXGIAdapter *pAdapter = nullptr;
				UINT nAdapterIndex = 0;

				while(!bHasActiveDisplay && SUCCEEDED(pFactory->EnumAdapters(nAdapterIndex++, &pAdapter)))
				{
					IDXGIOutput *pOutput = nullptr;
					UINT nOutputIndex = 0;

					while(SUCCEEDED(pAdapter->EnumOutputs(nOutputIndex++, &pOutput)))
					{
						if(SUCCEEDED(pOutput->GetDesc(&od)) && od.AttachedToDesktop)
						{
							bHasActiveDisplay = true;
							pOutput->Release();
							break;
						}

						pOutput->Release();
					}

					pAdapter->Release();
				}

				pFactory->Release();
			}
		}

		FreeLibrary(hDxgi);
	}

	return bHasActiveDisplay;
}

#elif __APPLE__
//#################################################################################################
ELidState GetLidState(void)
{
	ELidState eState = ELS_Unknown;

	CIOObject ioReg = IORegistryEntryFromPath(kIOMasterPortDefault, "IOService:/AppleACPIPlatformExpert/IOPMrootDomain");
	if(ioReg)
	{
		CCFObject cfProp = IORegistryEntryCreateCFProperty(ioReg, CFSTR(kAppleClamshellStateKey), kCFAllocatorDefault, 0);
		if(cfProp)
			eState = (cfProp.GetBoolean()) ? ELS_Closed : ELS_Open;
	}

	return eState;
}
#endif

NS_END
