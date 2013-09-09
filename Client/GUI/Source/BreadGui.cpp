
#include "BreadGui.h"
#include "BreadDeclaration.h"
#include "BreadWindowXmlSerialize.h"
#include "BreadUtils.h"
#include "BreadRenderPropertyManager.h"
#include "BreadCore.h"
#include "BreadMsgInfo.h"

namespace Bread
{
	/*
	 *	
	 */
	cGui::cGui() : m_D3DDevice(NULL), m_input(NULL), m_focusWindow(NULL), m_fontMgr(NULL)
	{
	}
	/*
	 *	
	 */
	cGui::~cGui()
	{
		clear();
	}
	/*
	 *	
	 */
	bool cGui::initialize(LPDIRECT3DDEVICE9 device, HINSTANCE hInstance, HWND hWnd)
	{
		assert(!m_input && _T("m_input must be NULL"));

		m_D3DDevice = device;
		m_input = new cInputSystem;
		m_input->initialize(hInstance, hWnd);
		m_fontMgr = new cFontManager;
		if (!m_fontMgr->initialize(device)) return false;

		return true;
	}
	/*
	 *	
	 */
	void cGui::clear()
	{
		safeDelete(m_input);
		safeDelete(m_fontMgr, true);
		m_windowList.clear();
	}
	/*
	 *	
	 */
	void cGui::update(float elapsedtime)
	{
		clearOverWindow();

		m_input->update();
		m_windowList.update(elapsedtime, m_input->getMouseX(), m_input->getMouseY());
	}
	/*
	 *	
	 */
	void cGui::render()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);
		//_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);

		m_windowList.render(m_D3DDevice);

		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	int cGui::renderDebugInfo(int y)
	{
		return m_input->renderDebugInfo(y);
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cGui::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		int result = 1;
		if (m_input)
			result = (result) ? m_input->wndProc(hWnd, msg, wParam, lParam) : result;
		
		return result;
	}
	/*
	*/
	void cGui::msgProc(sMsgInfo* /*msg*/)
	{
	}
	/*
	*/
	void cGui::enterFocus(cWindow* win)
	{
		leaveFocus();
		m_focusWindow = win;
		if (m_focusWindow)
			m_focusWindow->enterFocus();
	}
	/*
	*/
	void cGui::leaveFocus()
	{
		if (!m_focusWindow) return ;
		m_focusWindow->leaveFocus();
		m_focusWindow = NULL;
	}
}






















