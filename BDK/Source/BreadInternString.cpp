
#include "BreadInternString.h"
#include "BreadHashFunctions.h"
#include "BreadBucketHash.h"
#include "BreadInline.h"

namespace Bread
{
	implementSingleton(cInternString)

	/*
	*/
	cInternString::cInternString()
	{
		m_hash = new bucket;
	}
	/*
	*/
	cInternString::~cInternString()
	{
		safeDelete(m_hash);
	}
	/*
	*/
	string_id cInternString::getId(TCHAR const* str)
	{
		string_id id = DJBHash(str, _tcslen(str));
		if (!m_hash->is(id))
		{
			m_hash->insert(id, str);
		}
		return id;
	}
	/*
	*/
	void cInternString::getStr(string_id id, std_string* str)
	{
		static std_string emptyStr = _T("empty string");
		if (0 == id)
		{
			*str = _T("empty string");
		}
		else
		{
			m_hash->get(id, str);
		}
	}
	/*
	*/
	bool cInternString::equal(TCHAR const* str, string_id id)
	{
		return getId(str) == id;
	}
	/*
	*/
	bool cInternString::equal(TCHAR const* str1, TCHAR const* str2)
	{
		return getId(str1) == getId(str2);
	}
}