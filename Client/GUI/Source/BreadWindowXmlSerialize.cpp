
#include <tchar.h>
#include "BreadWindowXmlSerialize.h"
#include "BreadWindow.h"


namespace Bread
{
	/*
	 *	
	 */
	template <class T>
	cWindowXmlSerialize<T>::cWindowXmlSerialize() : m_root(NULL)	{}
	/*
	 *	
	 */
	template <class T>
	cWindowXmlSerialize<T>::~cWindowXmlSerialize()
	{
		assert(!m_root && _T("m_root must be NULL!"));
	}
	/*
	 *	
	 */
	template <class T>
	T* cWindowXmlSerialize<T>::getRoot()
	{
		T* temp = m_root;
		m_root = NULL;
		return temp;
	}
	/*
	 *	
	 */
	template <class T>
	bool cWindowXmlSerialize<T>::load(char const* filename)
	{
		if (!m_root)
			m_root = new T(true, NULL);

		TiXmlDocument doc(filename);
		if (!doc.LoadFile())
		{
			assert(0 && _T("Failed load xml file"));
			return false;
		}

		TiXmlElement* element = doc.FirstChildElement();
		for (; element; element = element->NextSiblingElement())
		{
			if (strcmp(element->Value(), XMLELEM_INCLUDE) == 0)
			{
				const char* filename = element->Attribute(XMLATTRI_FILE);
				if (filename)
					load(filename);
			}

			serializeXml(element, m_root);	
		}

		return true;
	}
	/*
	 *	
	 */
	template <class T>
	void cWindowXmlSerialize<T>::serializeXml(TiXmlElement* element, cWindow* parent)
	{
		static sXmlAttribute attri;
		char value[128];

		for (; element; element = element->NextSiblingElement())
		{
			strcpy_s(value, element->Value());

			if (strcmp(value, XMLELEM_WINDOW) == 0)
			{
				cWindow* win = new cWindow;
				parent->addChild(win);
				readXmlAttribute(element, attri);
				win->setXmlAttribute(attri);
				serializeXml(element->FirstChildElement(), win);
			}
			else if (strcmp(value, XMLELEM_TEXTURE) == 0)
			{
				readXmlAttribute(element, attri);
				parent->setXmlTextureAttribute(attri);
				serializeXml(element->FirstChildElement(), parent);
			}
			else if (strcmp(value, XMLELEM_BUTTON) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_CHECKBOX) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_COMBOBOX) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_EDITBOX) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_FORM) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_LISTBOX) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_RADIOBUTTON) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_SCROLLBAR) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_SLIDER) == 0)
			{
			}
			else if (strcmp(value, XMLELEM_STATICTEXT) == 0)
			{
			}
		}
	}
	/*
	 *	
	 */
	template <class T>
	void cWindowXmlSerialize<T>::readXmlAttribute(TiXmlElement* element, sXmlAttribute& attri)
	{
		attri.init();

		int value;
		const char* str = NULL;

		str = element->Attribute(XMLATTRI_FILE);	if (str) attri.m_file = str;
		str = element->Attribute(XMLATTRI_NAME);	if (str) attri.m_name = str;
		str = element->Attribute(XMLATTRI_CALL);	if (str) attri.m_call = str;
		str = element->Attribute(XMLATTRI_TYPE);	if (str) attri.m_type = str;
		element->QueryFloatAttribute(XMLATTRI_X, &attri.m_x);
		element->QueryFloatAttribute(XMLATTRI_Y, &attri.m_y);
		element->QueryFloatAttribute(XMLATTRI_W, &attri.m_w);
		element->QueryFloatAttribute(XMLATTRI_H, &attri.m_h);
		element->QueryFloatAttribute(XMLATTRI_TX, &attri.m_tx);
		element->QueryFloatAttribute(XMLATTRI_TY, &attri.m_ty);
		element->QueryFloatAttribute(XMLATTRI_TW, &attri.m_tw);
		element->QueryFloatAttribute(XMLATTRI_TH, &attri.m_th);
		element->QueryIntAttribute(XMLATTRI_GROUP_ID, &attri.m_groupId);
		element->QueryIntAttribute(XMLATTRI_TOOLTIP_ID, &attri.m_tooltipId);
		if (element->QueryIntAttribute(XMLATTRI_VIAIBLE, &value) == TIXML_SUCCESS) attri.m_visible = (value == 1) ? true : false;
		if (element->QueryIntAttribute(XMLATTRI_MOVABLE, &value) == TIXML_SUCCESS) attri.m_movable = (value == 1) ? true : false;
	}
}





