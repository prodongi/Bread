
#include "BreadOctree.h"
#include "BreadOctreeNode.h"
#include "BreadRenderData.h"
#include "BreadFrustum.h"
#include "BreadSerializeBase.h"

namespace Bread
{
	/*
	*	
	*/
	cOctree::cOctree() : m_root(NULL)
	{
	}
	/*
	*	
	*/
	cOctree::~cOctree()
	{
		clear();
	}
	/*
	*	
	*/
	void cOctree::clear()
	{
		if (m_root)
			delete m_root;
	}
	/*
	*	
	*/
	void cOctree::createRoot(sBox const* box)
	{
		assert(!m_root);

		m_root = new cOctreeNode;
		m_root->create(0, box);
	}
	/*
	*	
	*/
	bool cOctree::add(cRenderData* renderData, sBox const* box, bool isSceneObject, int maxDepth)
	{
		if (!m_root)
		{
			assert(m_root);
			return false;
		}
		if (box->isZero())
		{
			assert(0 && _T("box is zero"));
			return false;
		}

		sVector3 center;
		box->calcCenter(center);
		if (!m_root->isIn(center))
			return false;

		m_root->add(renderData, box, isSceneObject, maxDepth);

		return true;
	}
	/*
	 *	
	 */
	void cOctree::copy(cOctree const& srcData)
	{
		if (!m_root)
			m_root = new cOctreeNode;
		m_root->copy(srcData.m_root);
	}
	/*
	 *	
	 */
	void cOctree::add(cOctree const& cOctree)
	{
		m_root->add(cOctree.m_root);
	}
	/*
	 *	
	 */
	void cOctree::expandK()
	{
		assert(0 && "allreay done when export is");
		m_root->expandK(m_expandK);
	}
	/*
	 *	
	 */
	void cOctree::read(cSerializeBase* serialize)
	{
		serialize->readValue(m_expandK);

		assert(!m_root);
		m_root = new cOctreeNode;
		m_root->read(serialize);
	}
	/*
	 *	
	 */
	void cOctree::culling(cFrustum const* frustum)
	{
		if (checkIsNull(frustum))
			return ;
		m_root->culling(frustum);
	}
}



















