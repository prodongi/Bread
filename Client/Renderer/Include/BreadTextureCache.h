
#ifndef _BreadTextureCache_h_
#define _BreadTextureCache_h_

#include "BreadDataFactory.h"
#include "BreadTexture.h"
#include "BreadScheduler.h"
#include "BreadBucketHash.h"
#include "BreadFileHelper.h"
#include "BreadHelperClass.h"

namespace Bread
{
	#define TEXTURE_CACHE_BUCKET_SIZE 1000

	/*
		@date 2011.04.21
		@auth prodongi
		@desc 텍스춰 캐쉬 관리자
		@todo 
	*/
	class cTextureCache : public cDataFactory<BUID,
											  cTexture,
											  cBucketHash<BUID, cTexture*, TEXTURE_CACHE_BUCKET_SIZE>,
											  cSchedulerEmpty,
											  cThreadBase<BUID, cTexture, sTextureLoadInfo, cTextureCache> >
	{
		declareInstance(cTextureCache)

	public:
		cTextureCache();
		virtual ~cTextureCache();

	protected:
		virtual void createThread();
	};
}



#endif