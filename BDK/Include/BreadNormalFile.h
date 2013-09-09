
#ifndef _BreadNormalFile_h_
#define _BreadNormalFile_h_

#include "BreadFile.h"
#include <iostream>
#include <fstream>

namespace Bread
{
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cNormalFile : public cFile
	{
	public:
		cNormalFile();
		virtual ~cNormalFile();

		virtual cFile* create();
		virtual bool is(std_string const& filename) const;
		virtual bool open(std_string const& filename);
		virtual void close();
		virtual bool eof();
		virtual void getLine(char* buffer, size_t bufferSize);
		virtual void read(char* buffer, size_t bufferSize);

	protected:
		std::ifstream m_fin;		
	};
}



#endif