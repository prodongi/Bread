
#ifndef _BreadThreadBase_h_
#define _BreadThreadBase_h_

#include "BreadStringHelper.h"

namespace Bread
{
	#define LOADING_BACKGROUND	0
	#define LOADING_DIRECT		1
	#define LOADING_ACCESS		2
	
	/*
		@date 2010.00.00
		@auth prodongi
		@desc cThreadBase���� Info�� ���̴� base ����ü
		@todo 
	*/
	struct sThreadInfo
	{
		sThreadInfo() {}
		/// @brief �����ڸ� ���ؼ��� buid, filename�� ���� �� �� �ֵ��� �Ѵ�, makeCacheBuid�� ȣ���ϱ� ���ؼ�,
		sThreadInfo(BUID const& buid, std_string const& filename) : m_buid(buid), m_filename(filename)
		{
			makeCacheBuid();
		}
		virtual ~sThreadInfo() {}
		virtual void makeCacheBuid()
		{
			m_cacheBuid = cStringHelper::getSingleton()->makeFilenameToBUID(m_filename);
		}
		BUID const& getBuid() const				{	return m_buid;		}
		BUID const& getCacheBuid() const		{	return m_cacheBuid;	}
		std_string const& getFileName() const	{	return m_filename;	}

	protected:
		BUID m_buid;
		BUID m_cacheBuid;		/// @brief ���� ��θ� ������ ���ϸ��� �����(��ο� ��� ���� üũ�ϱ� ���ؼ�)
		std_string m_filename;
	};
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	typedef unsigned int (WINAPI *fcLoadingThreadProc)(LPVOID);

	template <class K, class V, class I, class D>
	class cThreadBase
	{
	public:
		typedef K KEY;
		typedef V VALUE;
		typedef I LOADING_INFO;
		typedef D DATA_FACTORY;

	public:
		cThreadBase() {}
		virtual ~cThreadBase() {}
		virtual void initialize(fcLoadingThreadProc /*_loadingThreadProc*/, DATA_FACTORY* /*dataFactory*/) {}
		virtual void finalize() {}
		virtual void addLoadingList(LOADING_INFO const& /*info*/) {}
		virtual void directLoading(LOADING_INFO const& /*info*/) {}
		virtual void merge() {}
	};
	
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	template <class K, class V, class I, class D>
	class cLoadingThreadEmpty : public cThreadBase<K, V, I, D>
	{
	public:
		cLoadingThreadEmpty() {}
		virtual ~cLoadingThreadEmpty() {}
	};
}



#endif