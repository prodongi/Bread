
#ifndef _BreadOctree_h_
#define _BreadOctree_h_

#include "BreadDeclaration.h"

namespace Bread
{
	class cOctreeNode;
	class cFrustum;
	class cRenderList;
	class cRenderData;
	class cSerializeBase;
	struct sBox;

	/**
	* \ingroup BreadMaxExport
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
	* \date 2009-11-04
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
	class cOctree
	{
	public:
		cOctree();
		~cOctree();

		void createRoot(sBox const* box);
		bool add(cRenderData* renderData, sBox const* box, bool isSceneObject, int maxDepth = -1);
		void clear();
		void copy(cOctree const& srcData);
		void add(cOctree const& octree);
		void expandK();
		void culling(cFrustum const* frustum);
		cOctreeNode const* getRoot() const	{	return m_root;	}

		void read(cSerializeBase* serialize);

	private:
		cOctreeNode* m_root;
		float m_expandK;
	};
}







#endif















