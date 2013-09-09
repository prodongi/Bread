
#ifndef _BreadInternString_h_
#define _BreadInternString_h_

#include <tchar.h>
#include "BreadDefinition.h"
#include "BreadSingleton.h"

#define _getInternString() Bread::cInternString::getSingleton()

namespace Bread
{
	template <typename KEY, typename V, size_t bucketSize> class cBucketHash;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 사용된 스트링을 저장하고, 스트링 비교 대신 스트링 해쉬 코드로 비교하기 위해 쓰임
		@todo 
	*/
	class cInternString
	{
		declareSingleton(cInternString)

	public:
		~cInternString();

		string_id getId(TCHAR const* str);
		void getStr(string_id id, std_string* str);
		bool equal(TCHAR const* str, string_id id);
		bool equal(TCHAR const* str1, TCHAR const* str2);

	private:
		enum {	BUCKET_SIZE = 100 };	/// 임의의 적당한 값
		typedef cBucketHash<string_id, std_string, BUCKET_SIZE> bucket;
		bucket* m_hash;
	};
}

#endif
