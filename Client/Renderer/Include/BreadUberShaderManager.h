
#ifndef _BreadUberShaderManager_h_
#define _BreadUberShaderManager_h_

#include <assert.h>
#include "BreadBucketHash.h"
#include "BreadUberShader.h"
#include "BreadD3DSystemClassify.h"
#include "BreadCriticalSection.h"
#include "BreadObjectFactory.h"

#define _getUberShaderMgr()	Bread::cUberShaderManager::getInstance()

namespace Bread
{	
	class cUberShaderFragment;

	#define UBERSHADERMANAGER_BUCKET_SIZE	100
	// @brief ubershaer type
	#define UBERSHADER_DEFAULT	_T("uberShader")
	#define UBERSHADER_WATER	_T("waterUberShader")
	#define UBERSHADER_CLOUD	_T("cloudUberShader")

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
	 * \date 2010-03-11
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
	class cUberShaderManager : public cBucketHash<BUID, cUberShader*, UBERSHADERMANAGER_BUCKET_SIZE>
	{
		declareInstance(cUberShaderManager)

	public:
		cUberShaderManager();
		virtual ~cUberShaderManager();
		virtual int renderDebugInfo(int y)	{	return y;	}
		virtual bool initialize();
		virtual void finalize() {	m_cs.finalize();	}
		virtual void update(float elapsedtime);
		virtual void registerObjectCallBack();

		cUberShader* getBase() const;
		/// @brief uber shader fragments로 조합된 쉐이더를 생성, type은 위에 정의됨
		cUberShader* createShader(std_string const& type, uint uberFragments);
		cUberShader* createShader(std_string const& type, std_string const& uberFragments);
		/// @brief 쉐이더가 할당되어 있지 않을 때 기본으로 쓰일 쉐이더
		void createBaseShader();

	protected:
		virtual void createFragment();
		/// @brief uber shader의 기본 쉐이더 로드
		virtual bool loadSourceEffectData();
		bool loadSourceEffectData(TCHAR const* type, TCHAR const* filename);
		void lock()		{	m_cs.lock();		}
		void unlock()		{	m_cs.unlock();	}

	private:
		LPD3DXEFFECTPOOL m_pool;
		cCriticalSection m_cs;
		cUberShaderFragment* m_fragment;
		cObjectFactory<std_string, cUberShader>	m_objFactory;
		cStdMapHelper<std_string, std::string, cStdMapHelperClear> m_srcEffectDatas;
		cUberShader* m_baseShader;
	};	
}









#endif