
#include "BreadFog.h"
#include "BreadD3DSystemClassify.h"
#include "BreadCore.h"
#include "BreadGui.h"


namespace Bread
{
	/*
	 *	
	 */
	 cFog::cFog()
	{
	}
	/*
	 *	
	 */
	cFog::~cFog()
	{
	}
	/*
	 *	
	 */
	void cFog::initialize(cEngineConfig const* config)
	{
		m_enable = config->getFogEnable();
		m_zNear = config->getFogZNear();
		m_zFar = config->getFogZFar();
		m_color = config->getFogColor();
		calculDensity();

		// 초기화 할때 한번만 렌더 스테이트를 설정해 주면 되므로 RenderProperty로 설정하지 않고
		// 직접 설정해준다
		
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		if (m_enable)
		{
			device->SetRenderState(D3DRS_FOGENABLE, TRUE);
			device->SetRenderState(D3DRS_FOGCOLOR, m_color);
			device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		}
		else
		{
			device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
	}
	/*
	 *	
	 */
	void cFog::calculDensity()
	{
		m_density[0] = m_zFar/(m_zFar - m_zNear);
		m_density[1] = -1.0f/(m_zFar - m_zNear);
	}
	/*
	 *	
	 */
	void cFog::update(float /*elapsedtime*/)
	{
		float offset = 5.0f;
		if (_getInputSystem()->isPressedKey(DIK_1))	
			m_zNear += offset;
		else if (_getInputSystem()->isPressedKey(DIK_2)) 
			m_zNear -= offset;

		if (_getInputSystem()->isPressedKey(DIK_3))	
			m_zFar += offset;
		else if (_getInputSystem()->isPressedKey(DIK_4)) 
			m_zFar -= offset;

		calculDensity();
	}
	/*
	 *	
	 */
	int cFog::renderDebugInfo(int y)
	{
		static TCHAR str[MAX_PATH];
		_getFontMgr()->drawD3DText(10, y+=16, _T("[fog]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		
		_stprintf(str, _T("zNear: %f"), m_zNear);
		_getFontMgr()->drawD3DText(10, y+=16, str);

		_stprintf(str, _T("zFar : %f"), m_zFar);
		_getFontMgr()->drawD3DText(10, y+=16, str);

		return y;
	}
}