

#ifndef _BreadCameraManager_h_
#define _BreadCameraManager_h_

#include <windows.h>
#include <tchar.h>
#include "BreadDeclaration.h"
#include "BreadCamera.h"

#define _getCameraMgr()	Bread::cCameraManager::getInstance()

namespace Bread
{
	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-09-05
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cCameraManager
	{
		declareInstance(cCameraManager)

	public:
		cCameraManager();
		~cCameraManager();

		void add(cCamera* camera);
		void del(TCHAR* name);
		void clear();
		void update(float elapsedtime);
		void setMain(TCHAR* name);
		void setActive(TCHAR* name);
		cCamera* get(TCHAR* name);
		cCamera* getMain() const	{	return m_main;	}
		cCamera* getActive() const	{	return m_active;	}
		LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
		void setTransform(LPDIRECT3DDEVICE9 device);
		int renderDebugInfo(int y);

	protected:
		typedef std::map<std_string, cCamera*> m_camera;
		typedef m_camera::iterator			  it_camera;
		m_camera m_list;
		cCamera* m_main;
		cCamera* m_active;
	};
}











#endif













