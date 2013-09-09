
#include "BreadNormalFile.h"
#include "BreadStringHelper.h"
#include "BreadFileHelper.h"
#include "BreadInternString.h"
#include <assert.h>

namespace Bread
{
	/*
	*/
	cNormalFile::cNormalFile()
	{
	}
	/*
	*/
	cNormalFile::~cNormalFile()
	{
		if (m_fin.is_open())
		{
			close();
		}
	}
	/*
	*/
	cFile* cNormalFile::create()
	{
		return new cNormalFile;
	}
	/*
	*/
	bool cNormalFile::is(std_string const& filename) const
	{
		return cFileHelper::is(filename);
	}
	/*
	*/
	bool cNormalFile::open(std_string const& filename)
	{
		m_fullName = 0;
		m_dir = 0;
		m_fullName = 0;

		if (m_fin.is_open())
		{
			assert(0 && _T("serialize already open"));
			close();
		}

		char temp[FILENAME_LEN];
		cStringHelper::getSingleton()->w2m(filename.c_str(), temp, FILENAME_LEN);

		m_fin.open(temp, std::ios::binary);
		if (!m_fin.is_open())
		{
			assert(0 && _T("Failed file open"));
			return false;
		}

		m_fullName = cInternString::getSingleton()->getId(filename.c_str());

		TCHAR dir[NAME_LEN];
		TCHAR fname[NAME_LEN];
		_tsplitpath(filename.c_str(), NULL, dir, fname, NULL);
		m_dir = cInternString::getSingleton()->getId(dir);
		m_fileName = cInternString::getSingleton()->getId(fname);

		return true;
	}
	/*
	*/
	void cNormalFile::close()
	{
		m_fin.close();
	}
	bool cNormalFile::eof()
	{
		return m_fin.eof();
	}
	void cNormalFile::getLine(char* buffer, size_t bufferSize)
	{
		m_fin.getline(buffer, bufferSize);
	}
	void cNormalFile::read(char* buffer, size_t bufferSize)
	{
		m_fin.read(buffer, bufferSize);
	}
}