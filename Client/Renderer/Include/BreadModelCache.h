
#ifndef _BreadModelCache_h_
#define _BreadModelCache_h_

#include "BreadBucketHash.h"
#include "BreadScheduler.h"
#include "BreadThreadBase.h"
#include "BreadSerializeBmd.h"
#include "BreadDataFactory.h"
#include "BreadVector3.h"

namespace Bread
{
	#define MODEL_CACHE_BUCKET_SIZE	100
	
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	struct sModelLoadInfo : public sThreadInfo
	{
		sModelLoadInfo() {}
		sModelLoadInfo(BUID const& buid, std_string const& filename) : 
								sThreadInfo(buid, filename), m_radian(0.0f), m_shareType(ST_NONE), m_isScene(false), m_isFadeIn(false) {}
		void operator = (sModelLoadInfo const& info)
		{
			m_buid		= info.m_buid;
			m_cacheBuid = info.m_cacheBuid;
			m_filename	= info.m_filename;
			m_pos		= info.m_pos;
			m_radian	= info.m_radian;
			m_isScene	= info.m_isScene;
			m_shareType = info.m_shareType;
			m_isFadeIn	= info.m_isFadeIn;
		}

		sVector3 m_pos;
		float m_radian;
		uint m_shareType;
		bool m_isScene;
		bool m_isFadeIn;
	};
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	struct sModelCache
	{
		sModelCache() : m_cacheData(NULL), m_data(NULL) {}
		~sModelCache()
		{
			safeDelete(m_cacheData);
			safeDelete(m_data);
		}
		virtual bool loadingCacheThread(sModelLoadInfo const& info);
		virtual BUID const& getBuid() const				{	return m_info.getBuid();		}
		virtual std_string const& getFileName() const	{	return m_info.getFileName();	}
		virtual BUID const& getCacheBuid() const		{	return m_info.getCacheBuid();	}
		sModelCache* copyingThread();
		void setLoadingInfo(sModelLoadInfo const& info);
		cSerializeBmd::sCacheData*  m_cacheData;
		cSerializeBmd::sData*		m_data;
		sModelLoadInfo				m_info;
	};
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cModelCache : public cDataFactory<BUID, 
											sModelCache,
											cBucketHash<BUID, sModelCache*, MODEL_CACHE_BUCKET_SIZE>,
											cSchedulerEmpty,
											cThreadBase<BUID, sModelCache, sModelLoadInfo, cModelCache> >
	{
	public:
		cModelCache() {}
		virtual ~cModelCache() {}
		virtual void mergeCopyData(sModelCache* copyData);

	protected:
		virtual void createThread();
	};
}



#endif