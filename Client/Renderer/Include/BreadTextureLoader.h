
#ifndef _BreadTextureLoader_h_
#define _BreadTextureLoader_h_

#include "BreadD3DHeader.h"
#include "BreadDeclaration.h"
#include "BreadThreadBase.h"

namespace Bread
{
	class cTextureCache;

	/*
	*	
	*/
	struct sTextureLoadInfo : public sThreadInfo
	{
		sTextureLoadInfo() : m_loadingType(0)	{}
		sTextureLoadInfo(BUID const& buid, std_string const& filename, int textureType, int loadingType) : sThreadInfo(buid, filename)
		{
			m_loadingType = loadingType;
			m_textureType = textureType;
		}
		virtual ~sTextureLoadInfo() {}
		void operator = (sTextureLoadInfo const& info)
		{
			m_buid			= info.m_buid;
			m_cacheBuid		= info.m_cacheBuid;
			m_filename		= info.m_filename;
			m_loadingType	= info.m_loadingType;
			m_textureType   = info.m_textureType;
		}
		int m_loadingType;
		int m_textureType;
	};

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-10-03
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cTextureLoader
	{
	public:
		cTextureLoader();
		virtual ~cTextureLoader();
		virtual bool loadingCacheThread(sTextureLoadInfo const& info);
		virtual BUID const& getBuid() const			{	return m_info.getBuid();		}
		virtual std_string const& getFileName()		{	return m_info.getFileName();	}
		virtual BUID const& getCacheBuid() const	{	return m_info.getCacheBuid();	}
		virtual int getLoadingType() const			{	return m_info.m_loadingType;	}
		virtual bool loadTexture(std_string const& /*filename*/, int /*textureType*/) = 0;
		void setLoadingInfo(sTextureLoadInfo const& info);

		/// @brief 파일명.확장자
		static TCHAR const* makeBUID(std_string const& filename);

	protected:
		virtual bool initialize(std_string const& filename, int textureType, int loadingType);

	protected:
		sTextureLoadInfo m_info;
		
		friend class cTextureManager;
	};
}

#endif









