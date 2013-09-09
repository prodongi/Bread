
#include "Bread2DTexture.h"
#include "BreadFileSystem.h"
#include "BreadD3DSystemClassify.h"
#include "BreadVector3.h"

namespace Bread
{
	/*
	*/
	bool s2DTexture::load(std_string const& filename)
	{
		std_string realPath;
		if (!_getFileSystem()->findRealPath(filename, realPath))
		{
			return false;
		}

		if (!checkHResultReturn(D3DXCreateTextureFromFile(cD3DSystem::getD3DDevice(), realPath.c_str(), &m_tex)))
			return false;

		trace(_T("load 2D texture : %s\n"), filename.c_str());

		return true;
	}
	/*
	*/
	bool s2DTexture::create(uint w, uint h, D3DFORMAT format)
	{
		if (m_tex)
		{
			safeRelease(m_tex);
			trace(_T("[warning] create 2DTexture : m_tex isn't null\n"));
		}

		if (!checkHResultReturn(cD3DSystem::getD3DDevice()->CreateTexture(w, h, 0, D3DUSAGE_DYNAMIC, format, D3DPOOL_DEFAULT, &m_tex, NULL)))
			return false;

		trace(_T("create 2D texture : %s\n"));

		return true;
	}
	/*
	*/
	void s2DTexture::getDesc(D3DSURFACE_DESC& desc)
	{
		if (FAILED(m_tex->GetLevelDesc(0, &desc)))
		{
			assert(0 && "Failed texture get level desc");
		}
	}
	/*
	*/
	bool s2DTexture::lock(sTextureLockRect& lockRect)
	{
		D3DLOCKED_RECT lockedRect;
		if (!checkHResultReturn(m_tex->LockRect(0, &lockedRect, NULL, 0)))
			return false;

		lockRect.m_pitch = lockedRect.Pitch;
		lockRect.m_data = lockedRect.pBits;

		return true;
	}
	/*
	*/
	bool s2DTexture::unlock()
	{
		return checkHResultReturn(m_tex->UnlockRect(0));
	}
	/*
	*/
	void s2DTexture::makeRandomleyRotateData(uint width, uint height)
	{
		sTextureLockRect lockRect;
		if (lock(lockRect))
		{
			sVector3 v;
			int r, g, b, a;
			DWORD* data = (DWORD*)lockRect.m_data;
			for (uint y = 0; y < height; ++y)
			{
				for (uint x = 0; x < width; ++x)
				{					
					v.x = cMath::rand(1.0f, 1000.0f);
					v.y = cMath::rand(1.0f, 1000.0f);
					v.z = cMath::rand(1.0f, 1000.0f);
					v.normalize();

					r = (int)(v.x * 255.0f);
					g = (int)(v.y * 255.0f);
					b = (int)(v.z * 255.0f);
					a = 255;

					int pitch = lockRect.m_pitch/4;
					data[y*pitch + x] = (a<<24)|(r<<16)|(g<<8)|(b<<0);
				}
			}

			unlock();
		}
	}
}