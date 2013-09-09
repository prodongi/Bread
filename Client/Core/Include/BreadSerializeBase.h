
#ifndef _BreadSerializeBase_h_
#define _BreadSerializeBase_h_

#include <iostream>
#include <fstream>
#include "BreadDeclaration.h"
#include "BreadFile.h"

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
	 * \date 2010-01-12
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
	class cSerializeBase
	{
	public:
		cSerializeBase();
		virtual ~cSerializeBase();

		bool open(std_string const& filename);
		void close();
		bool eof();
		string_id getFullName() const;
		string_id getFileName() const;
		string_id getDir() const;

		void getLine(char* buffer, size_t bufferSize);
		void readName(TCHAR* name);	/// @brief char를 읽어서 TCHAR로 변환
		void readNameW(TCHAR* name);/// @breif TCHAR를 바로 읽음
		void readBUID(BUID& value);
		void readBUIDW(BUID& value);
		void readBuffer(char* buffer, size_t bufferSize);
		template <class T> void readValue(T& value)
		{
			m_file->read((char*)&value, sizeof (T));
		}
		template <class T> void readValueList(T* valueList, int num)
		{
			m_file->read((char*)valueList, sizeof (T) * num);
		}

	private:
		cFile* m_file;
	};
}















#endif