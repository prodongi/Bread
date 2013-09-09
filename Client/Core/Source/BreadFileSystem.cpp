
#include "BreadFileSystem.h"
#include "BreadNormalFile.h"
#include "BreadPackedFile.h"
#include "BreadSerializeBase.h"
#include "BreadStringHelper.h"
#include "BreadInternString.h"
#include "BreadFileHelper.h"

namespace Bread
{
	implementInstance(cFileSystem)

	/*
	*/
	cFileSystem::sResourcePath::~sResourcePath()
	{
		m_path.clear();
	}
	/*
	*/
	void cFileSystem::sResourcePath::loadPath()
	{
		cSerializeBase base;
		if (!base.open(_T("BaseData\\resource.path")))
		{
			assert(0 && _T("Failed open resource.path"));
			return ;
		}

		/// 루트 폴더
		m_path.pushBack(_T(""));

		char buffer[MAX_PATH];

		TCHAR t_buffer[MAX_PATH];
		TCHAR param1[MAX_PATH];
		TCHAR param2[MAX_PATH];
		std_string temp;

		while (!base.eof())
		{
			base.getLine(buffer, MAX_PATH);
			_getStringHelper()->m2w(buffer, t_buffer, MAX_PATH);

			_stscanf(t_buffer, _T("%260s %260s"), param1, param2);

			temp = param1;
			temp += _T("\\");
			m_path.pushBack(temp);

			if (_getInternString()->equal(_T("true"), param2))
			{
				std::vector<std_string> dirList;
				cFileHelper::getSubDirList(param1, dirList);

				if (!dirList.empty())
				{
					m_path.pushBack(dirList);
					dirList.clear();
				}
			}
		}

		base.close();
	}
	/*
	*/
	bool cFileSystem::sResourcePath::findRealPath(cFile* file, std_string const& filename, std_string& realPath)
	{
		if (0 == m_path.getBucketSize())
		{
			if (file->is(filename.c_str()))
			{
				realPath = filename;
				return true;
			}
			return false;
		}

		int i = 0;
		std_string fname, data;
		for (m_path.begin(); !m_path.isEnd(); m_path.next(), ++i)
		{
			data = m_path.getIterData();
			fname = data + filename;
			if (file->is(fname.c_str()))
			{
				realPath = fname;
				return true;
			}
		}
		return false;
	}

	/*
	*/
	cFileSystem::cFileSystem() : m_list(NULL)
	{
		assignInstance(this)
	}
	/*
	*/
	cFileSystem::~cFileSystem()
	{
		assignInstance(NULL)
	}
	/*
	*/
	bool cFileSystem::initialize()
	{
		m_cs.initialize();
		m_list = new cBucketVector<cFile*>;
		initFileList();
		initResourcePath();
				
		return true;
	}
	/*
	*/
	void cFileSystem::initFileList()
	{
		cFile* file;

		file = new cNormalFile;
		m_list->pushBack(&file);

		file = new cPackedFile;
		m_list->pushBack(&file);
	}
	/*
	*/
	void cFileSystem::initResourcePath()
	{
		m_resourcePath.loadPath();
	}
	/*
	*/
	void cFileSystem::finalize()
	{
		safeDelete(m_list);
		m_cs.finalize();
	}
	/*
	*/
	cFile* cFileSystem::open(std_string const& filename)
	{
		std_string realPath;
		cFile* file = findFile(filename, realPath);
		if (file && !file->open(realPath))
		{
#ifdef _DEBUG
			trace(_T("open failed %s\n"), realPath.c_str());
#endif
			safeDelete(file);
			return NULL;
		}

#ifdef _DEBUG
		trace(_T("open successed %s\n"), realPath.c_str());
#endif
		return file;
	};
	/*
	*/
	cFile* cFileSystem::findFile(std_string const& filename, std_string& realPath)
	{
		lock();
		cFile* file = NULL;
		for (m_list->begin(); !m_list->isEnd(); m_list->next())
		{
			file = m_list->getIterData();
			if (m_resourcePath.findRealPath(file, filename, realPath))
			{
				cFile* f = file->create();
				unlock();
				return f;
			}
		}
		unlock();
		return NULL;
	}
	/*
	*/
	bool cFileSystem::findRealPath(std_string const& filename, std_string& realPath)
	{
		lock();

		cFile* file = NULL;
		for (m_list->begin(); !m_list->isEnd(); m_list->next())
		{
			file = m_list->getIterData();
			if (m_resourcePath.findRealPath(file, filename, realPath))
			{
				unlock();
				return true;
			}
		}
		unlock();
		return false;
	}
}