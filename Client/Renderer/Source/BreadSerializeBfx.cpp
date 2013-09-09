
#include "BreadSerializeBfx.h"
#include <iostream>
#include <fstream>
#include "BreadUtils.h"
#include "BreadUberShaderManager.h"
#include "BreadUberShaderFragment.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	 *	
	 */
	cSerializeBfx::cSerializeBfx()
	{
	}
	/*
	 *	
	 */
	cSerializeBfx::~cSerializeBfx()
	{
		m_list.clear();
	}
	/*
	 *	
	 */
	void cSerializeBfx::load(std_string const& filename)
	{
		if (!open(filename))
			return ;

		TCHAR wstrSecond[20];
		char buffer[MAX_PATH];
		TCHAR _mtlName[NAME_LEN];
		char strFirst[NAME_LEN], strSecond[NAME_LEN], mtlName[NAME_LEN];
		uint shaderFlag = 0;

		while (!eof())
		{
			getLine(buffer, MAX_PATH);
			if (buffer[0] == ';')
				continue;

			sscanf(buffer, "%64s %64s", strFirst, strSecond);

			if (0 == strcmp(strFirst, "[material]"))
			{
				mtlName[0] = 0x00;
				shaderFlag = 0;
			}
			else if (0 == strcmp(strFirst, "[end]"))
			{
				_getStringHelper()->m2w(mtlName, _mtlName, NAME_LEN);
				m_list.insert(std::make_pair(_mtlName, shaderFlag));
			}
			else if (0 == strcmp(strFirst, "name"))
			{
				strcpy(mtlName, strSecond);
			}
			else if (0 == strcmp(strFirst, "shader"))
			{
				_getStringHelper()->m2w(strSecond, wstrSecond, 20);
				shaderFlag |= _getUberShaderFragment()->stringToFragment(wstrSecond);
			}
		}

		close();
	}
	/*
	 *	
	 */
	BUID const& cSerializeBfx::getUberShader(std_string const& mtlName)
	{
		static BUID buidNone = _T("0");

		uint fragments = 0;
		std::map<std_string, uint>::iterator it = m_list.find(mtlName);
		if (it != m_list.end())
			fragments = it->second;
		cUberShader* shader = _getUberShaderMgr()->createShader(UBERSHADER_DEFAULT, fragments);
		return (shader) ? shader->getBuid() : buidNone;
	}
}
















