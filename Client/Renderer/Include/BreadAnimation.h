
#ifndef _BreadAnimation_h_
#define _BreadAnimation_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"

namespace Bread
{
	class cAnimationMotionSet;
	class cAnimationController;
	class cRenderData;

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
	 * \date 2009-12-04
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
	class cAnimation
	{
	public:
		cAnimation();
		~cAnimation();

		void initialize(TCHAR const* filename, int jointNum, bool useGpu);
		void finalize();
		void update(float elapsedtime);
		void skinning(cRenderData* renderData);
		void setMotion(BUID const& motionBuid);
		void renderBone(sVector3 const& pos);
		void copy(cAnimation const* srcData);
		propertyConstFunc(BUID, Buid, m_buid)
		bool getUseGpu() const;
		void loadMotionFile(TCHAR const* filename);
		
		static TCHAR const* makeBUID(std_string const& filename);

	private:
		void getMotionFileName(TCHAR const* filename, TCHAR* motionFileName);

	public:
		enum	{	MAX_SKIN_LINK_NUM = 4	};

	private:
		BUID m_buid;
		std_string m_filename;
		int m_motionNum;
		cAnimationMotionSet* m_motionSet;
		cAnimationController* m_controller;

		friend class cSerialize3D;
	};
}



#endif