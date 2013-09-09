
#ifndef _BreadWindowXmlSerialize_h_
#define _BreadWindowXmlSerialize_h_

#include "BreadStdHeader.h"
#include "../../../Tool/xmllib/tinystr.h"
#include "../../../Tool/xmllib/tinyxml.h"

namespace Bread
{
	class cWindow;

	#define XMLELEM_INCLUDE			"include"
	#define XMLELEM_WINDOW			"window"
	#define XMLELEM_BUTTON			"button"
	#define XMLELEM_CHECKBOX		"checkbox"
	#define XMLELEM_COMBOBOX		"combobox"
	#define XMLELEM_EDITBOX			"editbox"
	#define XMLELEM_FORM			"form"
	#define XMLELEM_LISTBOX			"listbox"
	#define XMLELEM_RADIOBUTTON		"radiobutton"
	#define XMLELEM_SCROLLBAR		"scrollbar"
	#define XMLELEM_SLIDER			"slider"
	#define XMLELEM_STATICTEXT		"statictext"
	#define XMLELEM_TEXTURE			"texture"
	#define XMLELEM_EVENT			"event"
	#define XMLELEM_TEXTURE_OVER	"texture_over"
	#define XMLELEM_TEXTURE_DOWN	"texture_down"
	#define XMLELEM_GROUP			"group"

	
	#define XMLATTRI_FILE		"file"
	#define XMLATTRI_X			"x"
	#define XMLATTRI_Y			"y"
	#define XMLATTRI_W			"w"
	#define XMLATTRI_H			"h"
	#define XMLATTRI_TX			"tx"
	#define XMLATTRI_TY			"ty"
	#define XMLATTRI_TW			"tw"
	#define XMLATTRI_TH			"th"
	#define XMLATTRI_NAME		"name"
	#define XMLATTRI_VIAIBLE	"visible"
	#define XMLATTRI_MOVABLE	"movable"
	#define XMLATTRI_CALL		"call"
	#define XMLATTRI_TYPE		"type"
	#define XMLATTRI_GROUP_ID	"groupid"
	#define XMLATTRI_TOOLTIP_ID	"tooltipId"

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
	 * \date 2009-08-23
	 *
	 * \author Administrator
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
	template <class T>
	class cWindowXmlSerialize
	{
	public:
		struct sXmlAttribute
		{
			void init()
			{
				m_file.clear();
				m_name.clear();
				m_call.clear();
				m_type.clear();
				m_x = m_y = m_w = m_h = 0.0f;
				m_tx = m_ty = m_tw = m_th = 0.0f;
				m_groupId = m_tooltipId = 0;
				m_visible = true;
				m_movable = false;
			}
			std::string m_file;
			std::string m_name;
			std::string m_call;
			std::string m_type;
			float m_x;
			float m_y;
			float m_w;
			float m_h;
			float m_tx;
			float m_ty;
			float m_tw;
			float m_th;
			int m_groupId;
			int m_tooltipId;
			bool m_visible;
			bool m_movable;
		};

	public:
		cWindowXmlSerialize();
		~cWindowXmlSerialize();

		bool load(char const* filename);
		T* getRoot();

	private:
		void serializeXml(TiXmlElement* element, cWindow* parent);
		void readXmlAttribute(TiXmlElement* element, sXmlAttribute& attri);
	private:
		T* m_root;
	};
}









#endif