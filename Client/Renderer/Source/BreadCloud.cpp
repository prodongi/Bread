
#include "BreadCloud.h"
#include "BreadVolumeCloud.h"
#include "BreadInline.h"

namespace Bread
{
	/*
	*/
	cCloud::cCloud() : m_volumeCloud(NULL)
	{
	}
	/*
	*/
	cCloud::~cCloud()
	{
	}
	/*
	*/
	bool cCloud::initialize()
	{
		/*
		m_volumeCloud = new cVolumeCloud;
		if (!m_volumeCloud->initialize())
			return false;
			*/
		return true;
	}
	/*
	*/
	void cCloud::finalize()
	{
		safeDelete(m_volumeCloud);
	}
	/*
	*/
	void cCloud::render()
	{
		if (m_volumeCloud)
			m_volumeCloud->render();
	}
}