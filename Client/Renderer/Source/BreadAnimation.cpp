
#include "BreadAnimation.h"
#include "BreadAnimationMotionSet.h"
#include "BreadAnimationController.h"
#include "BreadUtils.h"
#include "BreadRenderer.h"
#include "BreadRenderData.h"
#include "BreadSerializeBase.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	*	
	*/
	TCHAR const* cAnimation::makeBUID(std_string const& filename)
	{
		return _getStringHelper()->makeFilenameToBUID(filename);
	}
	/*
	 *	
	 */
	cAnimation::cAnimation() : m_motionSet(NULL),
							   m_motionNum(0),
							   m_controller(NULL)
	{		
	}
	/*
	 *	
	 */
	cAnimation::~cAnimation()
	{
		finalize();
	}
	/*
	*/
	void cAnimation::initialize(TCHAR const* filename, int jointNum, bool useGpu)
	{
		//
		m_buid = cAnimation::makeBUID(filename);
		m_filename = filename;
		//
		assert(!m_controller);
		m_controller = new cAnimationController;
		m_controller->initialize(jointNum, useGpu);
		//
		assert(!m_motionSet);
		m_motionSet = new cAnimationMotionSet;
	}
	/*
	*/
	void cAnimation::finalize()
	{
		safeDelete(m_controller);
		safeDeleteArray(m_motionSet);
	}
	/*
	 *	
	 */
	void cAnimation::update(float elapsedtime)
	{
		if (!m_controller)
			return ;

		m_controller->update(elapsedtime);
	}
	/*
	 *	
	 */
	void cAnimation::skinning(cRenderData* renderData)
	{
		if (!m_controller)
			return ;

		m_controller->skinning(renderData);
	}
	/*
	*/
	void cAnimation::setMotion(BUID const& motionBuid)
	{
		if (!m_controller)
			return ;

		cAnimationMotion* curMotion = m_motionSet->getMotion(motionBuid);
		m_controller->setMotion(motionBuid, curMotion, NULL);
	}
	/*
	*/
	void cAnimation::copy(cAnimation const* srcData)
	{
		m_buid		= srcData->m_buid;
		m_filename	= srcData->m_filename;
		m_motionNum = srcData->m_motionNum;

		assert(!m_controller);
		m_controller = new cAnimationController;
		m_controller->copy(srcData->m_controller);

		assert(!m_motionSet);
		m_motionSet = new cAnimationMotionSet;
		m_motionSet->copy(srcData->m_motionSet, m_controller->getJointNum());
	}
	/*
	*/
	bool cAnimation::getUseGpu() const
	{
		if (!m_controller)
			return false;

		return m_controller->getUseGpu();
	}
	/*
	*/
	void cAnimation::renderBone(sVector3 const& pos)
	{
		if (!m_controller)
			return ;

		m_controller->renderBone(pos);
	}
	/*
	*/
	void cAnimation::loadMotionFile(TCHAR const* filename)
	{
		TCHAR motionFileName[NAME_LEN];
		getMotionFileName(filename, motionFileName);

		cSerializeBase serialize;
		if (!serialize.open(motionFileName))
			return ;

		TCHAR motionBuid[NAME_LEN];
		char buffer[MAX_PATH];
		char cmd[10], str1[10], str2[10], str3[10], str4[10], str5[10], str6[10];
		while (!serialize.eof())
		{
			serialize.getLine(buffer, MAX_PATH);

			sscanf(buffer, "%s %s %s %s %s %s %s", cmd, str1, str2, str3, str4, str5, str6);

			if (cmd[0] == ':')
			{
				_getStringHelper()->m2w(cmd+1, motionBuid, NAME_LEN);
			}
			else if (0 == strcmp(cmd, "end"))
			{
				motionBuid[0] = 0x00;
			}
			else if (0 == strcmp(cmd, "loop"))
			{
				cAnimationMotion* motion = m_motionSet->get(motionBuid);
				if (motion)
					motion->setLoop(atoi(str1) == 1 ? true : false);
			}
			else if (0 == strcmp(cmd, "sound"))
			{
				//m_controller->addEvent(motion, cmd);
			}
		}

		serialize.close();
	}
	/*
	*/
	void cAnimation::getMotionFileName(TCHAR const* filename, TCHAR* motionFileName)
	{
		_tcsncpy(motionFileName, filename, NAME_LEN);
		TCHAR* d = _tcsstr(motionFileName, _T("."));
		if (d)
		{
			_tcscpy(d+1, _T("motion"));
		}
		else
		{
			assert(0 && _T("can't extract motion file name"));
		}
	}
}

























