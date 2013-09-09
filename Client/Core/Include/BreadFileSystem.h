
#ifndef _BreadFileSystem_h_
#define _BreadFileSystem_h_

#include <assert.h>
#include "BreadClientDefinition.h"
#include "BreadBucketVector.h"
#include "BreadCriticalSection.h"

#define _getFileSystem()	Bread::cFileSystem::getInstance()

namespace Bread
{
	class cFile;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc ���������� ���ϵ��� ���� �� ���̴� Ŭ����(�Ϲ� ����, ��ŷ�� ���� ���)
		@todo 
	*/
	class cFileSystem
	{
		declareInstance(cFileSystem)
		declareClassCreater(cFileSystem)

		virtual bool initialize();
		virtual void finalize();
		virtual cFile* open(std_string const& filename);
		bool findRealPath(std_string const& filename, std_string& realPath);

	protected:
		virtual cFile* findFile(std_string const& filename, std_string& realPath);
		virtual void initFileList();
		void initResourcePath();
		void lock()		{	m_cs.lock();	}
		void unlock()	{	m_cs.unlock();	}

	protected:
		struct sResourcePath
		{
			~sResourcePath();
			void loadPath();
			bool findRealPath(cFile* file, std_string const& filename, std_string& realpath);
			cBucketVector<std_string> m_path;
		};

		cBucketVector<cFile*>* m_list;
		sResourcePath m_resourcePath;
		cCriticalSection m_cs;
	};
}



#endif