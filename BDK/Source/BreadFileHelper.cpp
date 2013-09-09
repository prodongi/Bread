
#include "BreadStringHelper.h"
#include "BreadFileHelper.h"
#include <assert.h>
#include <iostream>
#include <fstream>

namespace Bread
{
	TCHAR cFileHelper::m_tchar[MAX_PATH] = { 0x00, };
	char cFileHelper::m_char[MAX_PATH] = { 0x00, };

	/*
	*/
	bool cFileHelper::is(std_string const& filename)
	{
		cStringHelper::getSingleton()->w2m(filename.c_str(), m_char, FILENAME_LEN);
		std::ifstream fin(m_char, std::ios_base::in | std::ios_base::binary);
		bool isOpen = false;
		if (fin.is_open())
		{
			fin.close();
			isOpen = true;
		}
		return isOpen;
	}
	/*
	 *	
	 */
	uint cFileHelper::getFileSize(std_string const& filename)
	{
		cStringHelper::getSingleton()->w2m(filename.c_str(), m_char, FILENAME_LEN);

		std::ifstream fin(m_char, std::ios_base::in | std::ios_base::binary);
		if (!fin.is_open())
		{
			assert(0 && _T("Failed getFileSize open"));
			return 0;
		}
		fin.seekg(0, std::ios_base::end);
		std::istream::pos_type pt = fin.tellg();

		fin.close();

		return (uint)pt;
	}
	/*
	 *	
	 */
	char const* cFileHelper::getFileNameInFullPass(char const* fullPass)
	{
		if (!fullPass || strlen(fullPass) == 0)
			return NULL;

		char _filename[NAME_LEN];
		char _expname[NAME_LEN];
		_splitpath(fullPass, NULL, NULL, _filename, _expname);
		sprintf(m_char, "%s%s", _filename, _expname);
/*
		std::string _fullPass = fullPass;
		std::string::size_type pos = 0;
		pos = _fullPass.find_last_of("\\");
		if (pos == std::string::npos)
			return NULL;
		strcpy(m_char, fullPass + pos + 1);
*/
		return m_char;
	}
	/*
	 *	
	 */
	char const* cFileHelper::getPassNameInFullPass(char const* fullPass)
	{
		if (!fullPass || strlen(fullPass) == 0)
			return NULL;

		_splitpath(fullPass, NULL, m_char, NULL, NULL);
/*
		std::string _fullPass = fullPass;
		std::string::size_type pos = 0;
		pos = _fullPass.find_last_of("\\");
		if (pos == std::string::npos)
			return NULL;

		strcpy(m_char, fullPass);
		m_char[pos] = 0x00;
*/
		return m_char;
	}
	/*
	 *	
	 */
	TCHAR const* cFileHelper::getFileNameInFullPassT(TCHAR const* fullPass)
	{
		if (!fullPass || _tcslen(fullPass) == 0)
			return NULL;

		TCHAR _filename[NAME_LEN];
		TCHAR _expname[NAME_LEN];
		_tsplitpath(fullPass, NULL, NULL, _filename, _expname);
		_stprintf(m_tchar, _T("%s%s"), _filename, _expname);
/*
		std::wstring _fullPass = fullPass;
		std::wstring::size_type pos = 0;
		pos = _fullPass.find_last_of(_T("\\"));
		if (pos == std::wstring::npos)
			return NULL;
		_tcscpy(m_tchar, fullPass + pos+1);
*/
		return m_tchar;
	}
	/*
	 *	
	 */
	TCHAR const* cFileHelper::getPassNameInFullPassT(TCHAR const* fullPass)
	{
		if (!fullPass || _tcslen(fullPass) == 0)
			return NULL;

		_tsplitpath(fullPass, NULL, m_tchar, NULL, NULL);
		
		/*
		std::wstring _fullPass = fullPass;
		std::wstring::size_type pos = 0;
		pos = _fullPass.find_last_of(_T("\\"));
		if (pos == std::wstring::npos)
			return NULL;

		_tcscpy(m_tchar, fullPass);
		m_tchar[pos] = 0x00;
		*/

		return m_tchar;
	}
	/*
	 *	
	 */
	void cFileHelper::createDirectory(TCHAR const* dirName)
	{
		TCHAR curDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, curDir);
		_tcscat(curDir, _T("\\"));
		_tcscat(curDir, dirName);
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(curDir))
		{
			CreateDirectory(curDir, NULL);
		}
	}
	/*
	*/
	void cFileHelper::getFileList(TCHAR const* directory, TCHAR const* exe, std::vector<std_string>& fileList, bool isSubDir)
	{
		WIN32_FIND_DATA findData;
		HANDLE hResult;

		std_string path = directory;
		path += _T("\\");
		path += _T("*.*");

		std_string _exe(_T("."));
		_exe += exe;

		hResult = FindFirstFile(path.c_str(), &findData);

		while (INVALID_HANDLE_VALUE != hResult)
		{
			if (findData.cFileName[0] != _T('.') || lstrlen(findData.cFileName) > 2)
			{
				path = directory;
				path += _T("\\");
				path += findData.cFileName;

				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (isSubDir)
						getFileList(path.c_str(), exe, fileList, isSubDir);
				}
				else
				{
					if (path.substr(path.length()-4) == _exe)
						fileList.push_back(path);
				}
			}

			if (!FindNextFile(hResult, &findData))
				break;

		};

		if (INVALID_HANDLE_VALUE != hResult)
			FindClose(hResult);
	}
	/*
	*/
	void cFileHelper::getSubDirList(TCHAR const* directory, std::vector<std_string>& dirList)
	{
		WIN32_FIND_DATA findData;
		HANDLE hResult;

		std_string path = directory;
		path += _T("\\");
		path += _T("*.*");

		hResult = FindFirstFile(path.c_str(), &findData);

		while (INVALID_HANDLE_VALUE != hResult)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (lstrlen(findData.cFileName) > 2)
				{
					path = directory;
					path += _T("\\");
					path += findData.cFileName;
					dirList.push_back(path + _T("\\"));
					getSubDirList(path.c_str(), dirList);
				}
			}

			if (!FindNextFile(hResult, &findData))
				break;

		};

		if (INVALID_HANDLE_VALUE != hResult)
			FindClose(hResult);
	}
}