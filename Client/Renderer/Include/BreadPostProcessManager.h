
#ifndef _BreadPostProcessManager_h_
#define _BreadPostProcessManager_h_

#include "BreadBucketVector.h"
#include "BreadPostEffectBase.h"

namespace Bread
{
	class cRenderer;

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
	 * \date 2010-03-22
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
	 * \todo process 1 -> scale ��ȭ -> process 2 -> scale ��ȭ -> process 3 -> ...
	 *
	 * \bug 
	 *
	 */
	class cPostProcessManager : public cBucketVector<cPostEffectBase*>
	{
		declareClassCreater(cPostProcessManager)

	public:
		enum ACTIVATE
		{
			MONOCHROME,
			SEPIA,
			INVERSE,
			GBLUR_H,	/// @brief gaussian blur horizontal
			GBLUR_V,	/// @brief gaussian blur verticle
			BLOOM_H,	/// @brief bloom blur horizontal
			BLOOM_V,	/// @brief bloom blur verticle
			SCALE_ORI,	/// @brief rtt ���� ũ��
			SCALE_HALF,	/// @breif rtt 1/2 ũ��
			SCALE_QUART,/// @brief rtt 1/4 ũ��
			BRIGHT_PASS,
			TONE_MAP,
			EDGE_DETECT,
			NORMAL_EDGE_DETECT,
			SSAO,		/// @brief screen spaced ambient occlusion
			DOF,
			LIGHT_SCATTERING,	/// @brief light scattering (god ray)
			DECAL,
			EFFECT_NUM
		};

		enum HELPER { HELPER_NORMAL, HELPER_DEPTH, HELPER_NUM };

	private:
		/// 
		struct sHelper
		{
			sHelper() : m_is(false), m_shader(NULL), m_rtt(NULL) {}
			~sHelper()	{	safeDelete(m_shader);	safeDelete(m_rtt);	}
			bool m_is;
			cShader* m_shader;
			cRenderToTextureBase* m_rtt;
		};

	public:
		virtual bool initialize(uint width, uint height);
		virtual void finalize();
		virtual void update(float elapsedtime);
		virtual void render2D();
		virtual int renderDebugInfo(int y);
		virtual void renderDebugScreen();

		void addActive(cPostProcessManager::ACTIVATE type);
		bool is() const		{	return !m_activeList.empty();	}
		void beginRttScene(bool clear, DWORD color = 0);
		void endRttScene();
		void renderHelper();
		void isHelperRender(cPostProcessManager::HELPER type, bool is);
		/// @breif m_helper�� �ؽ��縦 �����Ѵ�.
		cTexture* getHelperTexture(cPostProcessManager::HELPER type) const;

	protected:
		bool initRtt(uint width, uint height);
		bool initHelper(uint width, uint height);
		bool initPostEffect(uint width, uint height);
		void initUiScreen(uint width, uint height);
		bool initMember();
		virtual void renderActiveEffect();
		/// @brief rtt�� in�� out�� ��ü
		void swapRttInOut();
		/// @brief type�� ORI or HALF�϶� �ش� ����� �´� rtt�� �������ش�.		
		void findRttOut(uint type, int customScaleType = -1);
		void renderHelper(sHelper* helper);
		virtual cPostEffectBase* createPostEffect(uint type, uint width, uint height);
		/// @brief �����ҷ��� type�� ���� ����Ǿ� �ִ� Ÿ�Ե�� ���� ��ȿ ������ üũ
		void checkValidActive(uint type);
		/// @brief activeList�� type�� �ִ��� ����
		bool isActiveType(uint type);
		void clearActive();
		/// @brief m_rtt���� rtt�� ���� ���� cRenderToTextureBase�� ã�´�.
		cRenderToTextureBase* findNotEqualRtt(cRenderToTextureBase* rtt);

	protected:
		typedef std::vector<cPostEffectBase*>	vec_active;
		typedef vec_active::iterator			it_active;

		enum {	RTT_NUM = 2	};
		enum {	RTT_SCALE_ORI, RTT_SCALE_HALF, RTT_SCALE_QUART, RTT_SCALE_NUM };
		
		vec_active m_activeList;
		/// @brief scene�� �������� rtt
		cRenderToTextureBase* m_rttScene;
		cRenderToTextureBase* m_rtt[RTT_SCALE_NUM][RTT_NUM];
		/// @brief m_rttNormalSize or m_rttHalfSize�� �����͸� ���� �ִ� rtt
		cRenderToTextureBase* m_rttIn;
		cRenderToTextureBase* m_rttOut;

		sUiPlane m_screenScene;
		/// @brief m_rttOut�� screen(0:ori, 1:half)
		sUiPlane m_screenScale[RTT_SCALE_NUM];
		sUiPlane* m_screenOut;
		uint m_scaleType;

		sHelper* m_helper[HELPER_NUM];
	};
}












#endif