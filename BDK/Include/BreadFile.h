
#ifndef _BreadFile_h_
#define _BreadFile_h_

#include "BreadDefinition.h"

namespace Bread
{
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cFile
	{
	public:
		cFile();
		virtual ~cFile();

		string_id getFileName() const	{	return m_fileName;	}
		string_id getFullName() const	{	return m_fullName;	}
		string_id getDir() const		{	return m_dir;		}

		virtual cFile* create()									{	return NULL;	}
		virtual bool is(std_string const& /*filename*/) const	{	return false;	}
		virtual bool open(std_string const& /*filename*/)		{	return false;	}
		virtual void close()	{	}
		virtual bool eof()		{	return false;	}
		virtual void getLine(char* /*buffer*/, size_t /*bufferSize*/)	{}
		virtual void read(char* /*buffer*/, size_t /*bufferSize*/)		{}

	protected:
		string_id m_fileName;
		string_id m_fullName;
		string_id m_dir;
	};
}



#endif