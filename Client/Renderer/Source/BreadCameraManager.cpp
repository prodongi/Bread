
#include "BreadCameraManager.h"
#include "BreadCore.h"
#include "BreadGui.h"

namespace Bread
{
	implementInstance(cCameraManager)

	/*
	 *	
	 */
	cCameraManager::cCameraManager() : m_main(NULL), m_active(NULL)
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cCameraManager::~cCameraManager()
	{
		assignInstance(NULL)
		clear();
	}
	/*
	 *	
	 */
	void cCameraManager::add(cCamera* camera)
	{
		if (get(camera->getName()))
			return ;
		m_list[camera->getName()] = camera;
	}
	/*
	 *	
	 */
	cCamera* cCameraManager::get(TCHAR* name)
	{
		it_camera it = m_list.find(name);
		return (it == m_list.end()) ? NULL : it->second;
	}
	/*
	 *	
	 */
	void cCameraManager::del(TCHAR* name)
	{
		it_camera it = m_list.find(name);
		if (it != m_list.end())
			m_list.erase(it);
	}
	/*
	 *	
	 */
	void cCameraManager::setMain(TCHAR* name)
	{
		m_main = get(name);
	}/*
	 *	
	 */
	void cCameraManager::setActive(TCHAR* name)
	{
		m_active = get(name);
	}
	/*
	 *	
	 */
	void cCameraManager::clear()
	{
		safeClearMap<it_camera>(m_list);
	}
	/*
	 *	
	 */
	void cCameraManager::update(float elapsedtime)
	{
		it_camera it = m_list.begin();
		for (; it != m_list.end(); ++it)
			it->second->update(elapsedtime);
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cCameraManager::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		it_camera it = m_list.begin();
		for (; it != m_list.end(); ++it)
			it->second->wndProc(hWnd, msg, wParam, lParam);
		return 1;
	}
	/*
	 *	
	 */
	void cCameraManager::setTransform(LPDIRECT3DDEVICE9 device)
	{
		if (!m_active)
			return ;
		m_active->setTransform(device);
	}
	/*
	*/
	int cCameraManager::renderDebugInfo(int y)
	{
		_getFontMgr()->drawD3DText(10, y+=16, _T("[Camera]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		return m_active->renderDebugInfo(y);
	}
}















