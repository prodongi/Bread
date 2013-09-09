
#include "BreadEngineConfig.h"
#include "BreadD3DHeader.h"
#include "BreadUtils.h"
#include "BreadShadowManager.h"
#include "BreadStringHelper.h"
#include <iostream>
#include <fstream>

namespace Bread
{
	/*
	 *	
	 */
	cEngineConfig::cEngineConfig()
	{
		m_title = _T("Bread");
		m_width = 1024;
		m_height = 768;
		m_colorDepth = 32;
		m_frequency = 60;
		m_fullScreen = false;
		m_vSync = false;
		m_antialiasing = true;
		m_depthBuffer = true;
		m_useWBuffer = false;
		m_useHUD = false;
		m_multiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		m_multiSampleQuality = 0;
		m_rttMultiSampleFloat16 = false;
		m_rttMultiSampleType = D3DMULTISAMPLE_NONE;
		m_rttMultiSampleQuality = 0;
		m_zNear = 1.0f;
		m_zFar = 100000.0f;
		m_shadowType = cShadowManager::DEFAULT_SHADOW;
		m_shadowWidth = 2048;
		m_shadowHeight = 2048;
		m_fogEnable = false;
		m_fogZNear = 10.0f;
		m_fogZFar = 1000.0f;
		m_fogColor = 0xd8e3fe;
		m_weatherParticleNum = 500;
		m_weatherParticleBoxSize = 30.0f;
		m_weatherTexture = _T("Blob.dds");
	}
	/*
	 *	
	 */
	bool cEngineConfig::valueToBool(char const* value) const
	{
		return (strcmp(value, "true") == 0) ? true : false;
	}
	/*
	 *	
	 */
	bool cEngineConfig::load(std_string const& filename)
	{
		char _filename[FILENAME_LEN];
		_getStringHelper()->w2m(filename.c_str(), _filename, MAX_PATH);

		std::ifstream fin;
		fin.open(_filename);
		if (!fin.is_open())
		{
			assert(0 && "failed open engine config file");
			return false;
		}

		char buffer[MAX_PATH];
		char type[NAME_LEN];
		char value[NAME_LEN];
		TCHAR temp[MAX_PATH];
		
		while (!fin.eof())
		{
			fin.getline(buffer, MAX_PATH);
			sscanf(buffer, "%64s %64s", type, value);

			if (0 == strcmp(type, "title"))
			{
				_getStringHelper()->m2w(value, temp, NAME_LEN);
				m_title = temp;
			}
			else if (0 == strcmp(type, "width"))
			{
				m_width = atoi(value);
			}
			else if (0 == strcmp(type, "height"))
			{
				m_height = atoi(value);
			}
			else if (0 == strcmp(type, "colorDepth"))
			{
				m_colorDepth = atoi(value);
			}
			else if (0 == strcmp(type, "frequency"))
			{
				m_frequency = atoi(value);
			}
			else if (0 == strcmp(type, "fullScreen"))
			{
				m_fullScreen = valueToBool(value);
			}
			else if (0 == strcmp(type, "vSync"))
			{
				m_vSync = valueToBool(value);
			}
			else if (0 == strcmp(type, "antialiasing"))
			{
				m_antialiasing = valueToBool(value);
			}
			else if (0 == strcmp(type, "depthBuffer"))
			{
				m_depthBuffer = valueToBool(value);
			}
			else if (0 == strcmp(type, "useWBuffer"))
			{
				m_useWBuffer = valueToBool(value);
			}
			else if (0 == strcmp(type, "useHUD"))
			{
				m_useHUD = valueToBool(value);
			}
			else if (0 == strcmp(type, "multiSampleType"))
			{
				m_multiSampleType = (D3DMULTISAMPLE_TYPE)atoi(value);
			}
			else if (0 == strcmp(type, "multiSampleQuality"))
			{
				m_multiSampleQuality = atoi(value);
			}
			else if (0 == strcmp(type, "rttMultiSampleFloat16"))
			{
				m_rttMultiSampleFloat16 = valueToBool(value);
			}
			else if (0 == strcmp(type, "rttMultiSampleType"))
			{
				m_rttMultiSampleType = atoi(value);
			}
			else if (0 == strcmp(type, "rttMultiSampleQuality"))
			{
				m_rttMultiSampleQuality = atoi(value);
			}
			else if (0 == strcmp(type, "zNear"))
			{
				m_zNear = (float)atof(value);
			}
			else if (0 == strcmp(type, "zFar"))
			{
				m_zFar = (float)atof(value);
			}
			else if (0 == strcmp(type, "shadowType"))
			{
				_getStringHelper()->m2w(value, temp, NAME_LEN);
				m_shadowType = temp;
			}
			else if (0 == strcmp(type, "shadowWidth"))
			{
				m_shadowWidth = atoi(value);
			}
			else if (0 == strcmp(type, "shadowHeight"))
			{
				m_shadowHeight = atoi(value);
			}
			else if (0 == strcmp(type, "fogEnable"))
			{
				m_fogEnable = valueToBool(value);
			}
			else if (0 == strcmp(type, "fogZNear"))
			{
				m_fogZNear = (float)atof(value);
			}
			else if (0 == strcmp(type, "fogZFar"))
			{
				m_fogZFar = (float)atof(value);
			}
			else if (0 == strcmp(type, "fogColor"))
			{
				m_fogColor = hexToInt(value);
			}
			else if (0 == strcmp(type, "weatherParticleNum"))
			{
				m_weatherParticleNum = atoi(value);
			}
			else if (0 == strcmp(type, "weatherParticleBoxSize"))
			{
				m_weatherParticleBoxSize = (float)atof(value);
			}
			else if (0 == strcmp(type, "weatherTexture"))
			{
				_getStringHelper()->m2w(value, temp, NAME_LEN);
				m_weatherTexture = temp;
			}
		}

		fin.close();

		return true;
	}
}