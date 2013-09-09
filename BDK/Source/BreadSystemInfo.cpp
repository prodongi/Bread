
#include "BreadSystemInfo.h"
#include "BreadCpuInfo.h"
#include "BreadDtwinver.h"
#include "BreadUtils.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	*	
	*/
	void cSystemInfo::getSystemInfo(std::string& str, unsigned int flag)
	{
		if (SI_DISPLAY & flag)		getDisplayInfo(str);
		if (SI_OS & flag)			getOsInfo(str);
		if (SI_CPU & flag)			getCpuInfo(str);
		if (SI_DRIVE_SPACE & flag)	getDriveSpace(str);
		if (SI_RAM & flag)			getRamInfo(str);
		if (SI_DX_VERSION & flag)	getDirectXVersion(str);
		if (SI_SOUND & flag)		getSoundInfo(str);

		str += "\n";
	}
	/*
	*	
	*/
	void cSystemInfo::getDisplayInfo(std::string& str)
	{
		str += "\n\n[DISPLAY INFO]";

		DISPLAY_DEVICE dispDev;
		ZeroMemory( &dispDev, sizeof(dispDev) );
		dispDev.cb = sizeof( dispDev );

		char strR[MAX_PATH];

		if (TRUE == EnumDisplayDevices(NULL, 0, &dispDev, 0))
		{
			char szDeviceString[128];
			/// display name
			cStringHelper::getSingleton()->w2m(dispDev.DeviceString, szDeviceString, 128);
			str += "\nDisplay Name : ";
			str += szDeviceString;

			/// resolution
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);
			sprintf(strR, "Width : %d, Height : %d", width, height);
			str += "\nScreen Resolution : ";
			str += strR;
		}
		else
		{
			str += "\nFailed Enumerate Display Device";
		}

		DEVMODE devMode;
		ZeroMemory(&devMode, sizeof (devMode));
		devMode.dmSize = sizeof (devMode);
		if (TRUE == EnumDisplaySettings(NULL, 0, &devMode))
		{
			/// pixel depth
			sprintf(strR, "\nColor Depth : %d Bits Per Pixel", devMode.dmBitsPerPel);
			str += strR;
		}
		else
		{
			str += "\nFailed Enumerate Display Setting";
		}
	}
	/*
	*	
	*/
	void cSystemInfo::getOsInfo(std::string& str)
	{
		str += "\n\n[OS INFO]";

		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if( FALSE == GetVersionEx ((OSVERSIONINFO *) &osvi))
		{
			str += "\nFailed GetVersionEx";
			return ;
		}

		GetSystemInfo(&si);

		char buffer[MAX_PATH];
		DWORD majorVer = osvi.dwMajorVersion;
		DWORD minorVer = osvi.dwMinorVersion;

		if (VER_PLATFORM_WIN32s == osvi.dwPlatformId)
		{
			str += "\nMicrosoft ";
			str += "\nWindows 3.1";
		}
		else if (VER_PLATFORM_WIN32_WINDOWS == osvi.dwPlatformId)
		{
			str += "\nMicrosoft ";

			if (majorVer == 4)
			{
				if (minorVer == 0)
				{
					str += "Windows 95";
				}
				else if (minorVer == 10)
				{
					str += "Windows 98";
				}
				else if (minorVer == 90)
				{
					str += "Windows Me";
				}
				else
				{
					sprintf(buffer, "Invalid minor Version : %d / VER_PLATFORM_WIN32_WINDOWS, major version : %d", minorVer, majorVer);
					str += buffer;
					return ;
				}
			}
			else
			{
				sprintf(buffer, "Invalid major Version : %d / VER_PLATFORM_WIN32_WINDOWS, minor version : %d", majorVer, minorVer);
				str += buffer;
				return ;
			}
		}
		else if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId)
		{
			str += "\nMicrosoft ";

			if (majorVer == 3)
			{
				sprintf(buffer, "Invalid major Version : %d / VER_PLATFORM_WIN32_NT, minor version : %d", majorVer, minorVer);
				str += buffer;
				return ;
			}
			else if (majorVer == 4)
			{
				sprintf(buffer, "Invalid major Version : %d / VER_PLATFORM_WIN32_NT, minor version : %d", majorVer, minorVer);
				str += buffer;
				return ;
			}
			else if (majorVer == 5)
			{
				if (minorVer == 0)
				{
					str += "Windows 2000 ";

					if ( osvi.wProductType == VER_NT_WORKSTATION )
					{
						str += "Professional";
					}
					else 
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							str += "Datacenter Server";
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							str += "Advanced Server";
						else str += "Server";
					}
				}
				else if (minorVer == 1)
				{
					str += "Windows XP ";

					if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
						str += "Home Edition";
					else 
						str += "Professional";
				}
				else if (minorVer == 2)
				{
					if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
						str += "Windows XP Professional x64 Edition";
					else 
						str += "Windows Server 2003, ";

					// Test for the server type.
					if ( osvi.wProductType != VER_NT_WORKSTATION )
					{
						if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
						{
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
								str += "Datacenter Edition for Itanium-based Systems";
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
								str += "Enterprise Edition for Itanium-based Systems";
						}
						else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
						{
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
								str += "Datacenter x64 Edition";
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
								str += "Enterprise x64 Edition";
							else 
								str += "Standard x64 Edition";
						}
						else
						{
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
								str += "Datacenter Edition";
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
								str += "Enterprise Edition";
							else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
								str += "Web Edition";
							else 
								str += "Standard Edition";
						}
					}
				}
			}
			else if (majorVer == 6)
			{
				if (minorVer == 0)
				{
					if( osvi.wProductType == VER_NT_WORKSTATION )
						str += "Windows Vista ";
					else 
						str += "Windows Server 2008 ";

					if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
						str += ", 64-bit";
					else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
						str += ", 32-bit";
				}
			}
			else
			{
				sprintf(buffer, "Invalid major Version : %d / VER_PLATFORM_WIN32_NT, minor version : %d", majorVer, minorVer);
				str += buffer;
				return ;
			}		
		}
		else
		{  
			str += "\nThis sample does not support this version of Windows.";
			return ;
		}

		// Include service pack (if any) and build number.

		if( _tcslen(osvi.szCSDVersion) > 0 )
		{
			char szStr[128];
			cStringHelper::getSingleton()->w2m(osvi.szCSDVersion, szStr, 128);
			str += " ";
			str += szStr;
		}

		sprintf(buffer, " (build %d)", osvi.dwBuildNumber);
		str += buffer;
	}
	/*
	*	
	*/
	void cSystemInfo::getCpuInfo(std::string& str)
	{
		str += "\n\n[PROCESSOR INFO]";

		SYSTEM_INFO sysInfo;

		ZeroMemory (&sysInfo, sizeof (SYSTEM_INFO));
		GetSystemInfo(&sysInfo);

		/// number of cpu
		char n[32];
		sprintf(n, "%d", sysInfo.dwNumberOfProcessors);
		str += "\nNumber Of nProcessor : ";
		str += n;

		/// cpu name
		str += "\nProcessor Name : ";
		CPUInfo cpuInfo;
		if (cpuInfo.DoesCPUSupportCPUID())
		{
			str += cpuInfo.GetExtendedProcessorName();
		}
		else
		{
			str += "Error: CPU does not support CPUID.";
		}
	}
	/*
	*	
	*/
	void cSystemInfo::getRamInfo(std::string& str)
	{
		str += "\n\n[RAM INFO]";

		MEMORYSTATUS memoryStatus;
		ZeroMemory(&memoryStatus,sizeof(MEMORYSTATUS));
		memoryStatus.dwLength = sizeof (MEMORYSTATUS);

		::GlobalMemoryStatus (&memoryStatus);

		char buffer[MAX_PATH];

		/// total memory
		sprintf(buffer, "\r\nMemory Available: %ldKB",(DWORD) (memoryStatus.dwAvailPhys/1024));
		str += buffer;

		sprintf(buffer, "\r\nPrecent of used RAM: %ld%%",memoryStatus.dwMemoryLoad);
		str += buffer;

		/// free memory
		sprintf(buffer, "\nFree RAM: %ld%%",100-memoryStatus.dwMemoryLoad);
		str += buffer;
	}
	/*
	*	
	*/
	void cSystemInfo::getDriveSpace(std::string& str)
	{
		str += "\n\n[DRIVE SPACE INFO]";

		ULARGE_INTEGER AvailableToCaller, Disk, Free;
		char buffer[MAX_PATH];

		for (int iCounter=0;iCounter<MAX_OF_HARD_DISKS;iCounter++)
		{
			if (GetDriveTypeA(HardDiskLetters[iCounter])==DRIVE_FIXED)
			{
				if (GetDiskFreeSpaceExA(HardDiskLetters[iCounter],&AvailableToCaller, &Disk, &Free))
				{
					sprintf(buffer, "\n*** Hard Disk: (%s) ***\r",HardDiskLetters[iCounter]);
					str += buffer;

					sprintf(buffer, "\nTotal size: %I64d (MB)\r",Disk.QuadPart/1024/1024);
					str += buffer;

					ULONGLONG Used=Disk.QuadPart-Free.QuadPart;
					sprintf(buffer, "\nUsed: %I64d (MB)\r",Used/1024/1024);
					str += buffer;

					sprintf(buffer, "\nFree: %I64d (MB)\r",Free.QuadPart/1024/1024);
					str += buffer;
				}

				char Label[256]="";
				char FS[256]="";
				DWORD SerialNumber=0, MaxFileLen=0, FileSysFlag=0;

				if (GetVolumeInformationA(HardDiskLetters[iCounter], Label, 256, &SerialNumber, &MaxFileLen, &FileSysFlag, FS, 256))
				{
					sprintf(buffer, "\nMax of File Length Support: %d\r", MaxFileLen);
					str += buffer;

					sprintf(buffer, "\nFile System: %s\r\r", FS);
					str += buffer;
				}
			}
		}
	}
	/*
	*	
	*/
	void cSystemInfo::getDirectXVersion(std::string& str)
	{
		str += "\n\n[DIRECTX VERSION INFO]";

		HRESULT hr;
		TCHAR strResult[128];

		DWORD dwDirectXVersion = 0;
		TCHAR strDirectXVersion[10];

		hr = getDXVersion( &dwDirectXVersion, strDirectXVersion, 10 );
		if( SUCCEEDED(hr) )
		{
			if( dwDirectXVersion > 0 )
				_sntprintf( strResult, 128, TEXT("DirectX %s installed"), strDirectXVersion );
			else
				_tcsncpy( strResult, TEXT("DirectX not installed"), 128 );
			strResult[127] = 0;
		}
		else
		{
			_sntprintf( strResult, 128, TEXT("Unknown version of DirectX installed"), hr );
			strResult[127] = 0;
		}

		char szResult[128];
		cStringHelper::getSingleton()->w2m(strResult, szResult, 128);
		str += "\n";
		str += szResult;
	}
	/*
	*	
	*/
	void cSystemInfo::getSoundInfo(std::string& str)
	{
		str += "\n\n[SOUND INFO]";

		UINT waveDevNum = waveOutGetNumDevs();
		if (waveDevNum == 0)
		{
			str += "\nSound device number is Zero";
			return ;
		}

		char buffer[MAX_PATH];
		WAVEOUTCAPS caps;
		for (UINT i = 0; i < waveDevNum; ++i)
		{
			waveOutGetDevCaps(0, &caps, sizeof (caps));

			sprintf(buffer, "\n[%d] %s", i, caps.szPname);
			str += buffer;
		}	
	}
	/*
	*/
	HRESULT cSystemInfo::getDXVersion(DWORD* /*pdwDirectXVersion*/, 
									  TCHAR* /*strDirectXVersion*/, 
									  int /*cchDirectXVersion*/)
	{
		return 0;
	}
}
















