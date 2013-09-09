
#ifndef _BreadSystemInfo_h_
#define _BreadSystemInfo_h_

#include <windows.h>
#include "BreadStdHeader.h"

namespace Bread
{
#define MAX_OF_HARD_DISKS	24
	static char HardDiskLetters[MAX_OF_HARD_DISKS][4]={
		"c:\\",	"d:\\",	"e:\\",	"f:\\",	"g:\\",	"h:\\",
		"i:\\",	"j:\\",	"k:\\",	"l:\\",	"m:\\",	"n:\\",
		"o:\\",	"p:\\",	"q:\\",	"r:\\",	"s:\\",	"t:\\",
		"u:\\",	"v:\\",	"w:\\",	"x:\\",	"y:\\",	"z:\\"
	};


	class cSystemInfo
	{
	public:
		enum
		{
			SI_DISPLAY = 1,
			SI_OS = 2,
			SI_CPU = 4,
			SI_RAM = 8,
			SI_DRIVE_SPACE = 16,
			SI_DX_VERSION = 32,
			SI_SOUND = 64,
			SI_ALL = SI_DISPLAY | SI_OS | SI_CPU | SI_RAM | SI_DRIVE_SPACE | SI_DX_VERSION | SI_SOUND,
		};
		cSystemInfo() {}
		virtual ~cSystemInfo() {}

		void getSystemInfo(std::string& str, unsigned int flag);

	protected:
		virtual void getDisplayInfo(std::string& str);
		virtual void getOsInfo(std::string& str);
		virtual void getCpuInfo(std::string& str);
		virtual void getRamInfo(std::string& str);
		virtual void getDriveSpace(std::string& str);
		virtual void getDirectXVersion(std::string& str);
		virtual void getSoundInfo(std::string& str);

		virtual HRESULT getDXVersion(DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion);
	};
}







#endif