
#ifndef _BreadWindow_h_
#define _BreadWindow_h_

#include <tchar.h>
#include "BreadD3DHeader.h"
#include "BreadWindowList.h"
#include "BreadDeclaration.h"
#include "BreadWindowRenderData.h"
#include "BreadRect2.h"
#include "BreadWindowXmlSerialize.h"

namespace Bread
{
	/**
	 * \ingroup GUI
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
	 * \date 2009-08-22
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
	class cWindow : public cWindowRenderData
	{
	public:
		cWindow();
		cWindow(cWindow* parent);
		cWindow(bool root, cWindow* parent);
		virtual ~cWindow();

		propertyFunc(float, X, m_posRect.m_pos.x)
		propertyFunc(float, Y, m_posRect.m_pos.y)
		propertyFunc(float, Width, m_posRect.m_width)
		propertyFunc(float, Height, m_posRect.m_height)
		propertyFunc(int, TooltipId, m_tooltipId)
		propertyFunc(int, WinType, m_winType)
		propertyFunc(cWindow*, Parent, m_parent)
		propertyFunc(bool, Show, m_show)
		propertyFunc(bool, Enable, m_enable)
		propertyConstFunc(cRect2, PosRect, m_posRect)

		HRESULT createVB();

		void addChild(cWindow* win)	{	m_childList.add(win);	}
		void toggleShow() 			{	setShow(!getShow());	}
		cWindow* findChild(std_string const& key) const	{	return m_childList.find(key);	}

		virtual void update(float elapsedtime, int x, int y);
		virtual void render(LPDIRECT3DDEVICE9 device);
		virtual void setXmlAttribute(cWindowXmlSerialize<cWindow>::sXmlAttribute const& attri);
		virtual void setXmlTextureAttribute(cWindowXmlSerialize<cWindow>::sXmlAttribute const& attri);
		virtual void enterFocus() {	m_focus = true;	}
		virtual void leaveFocus() {	m_focus = false;}

		void updateChildList(float elapsedtime, int x, int y)	{	m_childList.update(elapsedtime, x, y);	}
		void renderChildList(LPDIRECT3DDEVICE9 device)	{	m_childList.render(device);			}
		void setUiName(TCHAR const* name)				{	_tcsncpy(m_uiName, name, WINDOW_NAME_LEN);	}
		void setTexName(TCHAR const* name)				{	_tcsncpy(m_texName, name, WINDOW_NAME_LEN);	}
		TCHAR const* getUiName() const					{	return m_uiName;		}
		bool isIn(int x, int y)							{	return m_posRect.isIn(x, y);		}
		float getParentX() const						{	return (m_parent) ? m_parent->getX() : 0.0f;	}
		float getParentY() const						{	return (m_parent) ? m_parent->getY() : 0.0f;	}

	protected:

	public:
		static int const WINDOW_NAME_LEN = 32;

		enum
		{
			UI_WINDOW = 1,
			UI_BACKGROUND,
			UI_EDITBOX,
			UI_SLIDER,
			UI_BUTTON,
			UI_LISTBOX,
			UI_STATICTEXT,
			UI_CHECKBOX,
			UI_RADIOBUTTON,
			UI_TOOLTIP,
			UI_COMBOBOX,
			UI_SCROLLBAR,
			UI_FORM,
		};

	protected:
		int   m_winType;
		cRect2 m_posRect;
		cRect2 m_texRect;
		float m_texWidth, m_texHeight;
		TCHAR m_uiName[WINDOW_NAME_LEN];
		TCHAR m_texName[WINDOW_NAME_LEN];
		int   m_tooltipId;
		/// root는 아무런 데이타가 없는 최상위 윈도우 이다
		bool m_root;
		bool m_show;
		bool m_enable;
		bool m_focus;
		cWindow* m_parent;

		cWindowList<cWindow> m_childList;
	};
}





#endif