
#include "BreadScreenShot.h"
#include "BreadFileHelper.h"
#include "BreadD3DSystemClassify.h"
#include "BreadCore.h"
#include "BreadGui.h"

namespace Bread
{
	uint cScreenShot::expType = D3DXIFF_JPG;
	std_string cScreenShot::directoryName = _T("ScreenShot");
	unsigned char cScreenShot::captureKey = DIK_P;

	/*
	 *	
	 */
	cScreenShot::cScreenShot()
	{
	}
	/*
	 *	
	 */
	cScreenShot::~cScreenShot()
	{
	}
	/*
	 *	
	 */
	void cScreenShot::initialize()
	{
		cFileHelper::createDirectory(cScreenShot::directoryName.c_str());
		getCurrentCount();
	}
	/*
	 *	
	 */
	void cScreenShot::update()
	{
		if (_getInputSystem()->isJustPressedKey(cScreenShot::captureKey))
		{
			capture();
		}
	}
	/*
	 *	
	 */
	void cScreenShot::getCurrentCount()
	{
		TCHAR fileDir[MAX_PATH];
		_stprintf(fileDir, _T("%s\\*%s"), cScreenShot::directoryName.c_str(), cScreenShot::getExpName());

		m_count = 0;
		WIN32_FIND_DATA fd;
		HANDLE hff = FindFirstFile(fileDir, &fd);
		if (hff != INVALID_HANDLE_VALUE)
		{
			bool is = true;
			while (is)
			{
				if (fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				{
					int count = calculCount(fd.cFileName);
					if (count > m_count)
						m_count = count;
				}

				if (FindNextFile(hff, &fd) == FALSE)
					break;
			}
		}

		m_count += 1;

		if (hff != INVALID_HANDLE_VALUE)
			FindClose(hff);
	}
	/*
	 *	
	 */
	int cScreenShot::calculCount(TCHAR const* pathName)
	{
		TCHAR fileName[FILENAME_LEN];
		_tcscpy(fileName, cFileHelper::getFileNameInFullPassT(pathName));

		TCHAR* exp = _tcsstr(fileName, cScreenShot::getExpName());
		exp[0] = 0x00;
		TCHAR* f = &fileName[_tcslen(cScreenShot::directoryName.c_str())];
		return _tstoi(f);
	}
	/*
	 *	
	 */
	TCHAR const* cScreenShot::getExpName()
	{
		switch (cScreenShot::expType)
		{
		case D3DXIFF_BMP:	return _T(".bmp");
		case D3DXIFF_JPG:	return _T(".jpg");
		case D3DXIFF_TGA:	return _T(".tga");
		case D3DXIFF_PNG:	return _T(".png");
		case D3DXIFF_DDS:	return _T(".dds");
		case D3DXIFF_PPM:	return _T(".ppm");
		case D3DXIFF_DIB:	return _T(".dib");
		default:			return _T(".jpg");
		}
	}
	/*
	 *	
	 */
	void cScreenShot::capture()
	{
		TCHAR szCount[MAX_PATH];
		_stprintf(szCount, _T("%d"), m_count);
		//m_count의 자리수를 구한다
		int data = m_count;
		int p = 0;
		bool is = true;
		while (is)
		{
			int mod = data/10;
			if (mod == 0) 
				break;

			data = mod;
			++p;
		}
		p += 1;

		//최대 자릿수
		int max_p = 4;
		int last_p = max_p - p;

		std_string str;
		str = cScreenShot::directoryName + _T("\\ScreenShot");
		for (int i = 0; i < last_p; ++i)
			str += _T("0");
		str += szCount;
		str += cScreenShot::getExpName();

		saveToFile(str.c_str());

		++m_count;
	}
	/*
	 *	
	 */
	void cScreenShot::saveToFile(TCHAR const* fileName)
	{
		LPDIRECT3DSURFACE9 pbackbuffer;
		if (cD3DSystem::getD3DDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pbackbuffer) == D3D_OK)
		{
			D3DXSaveSurfaceToFile(fileName, (D3DXIMAGE_FILEFORMAT)cScreenShot::expType, pbackbuffer, NULL, NULL);
			pbackbuffer->Release();

			trace(_T("screen shot: %s\n"), fileName);
		}
	}
}