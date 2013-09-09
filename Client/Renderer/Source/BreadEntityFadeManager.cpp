
#include "BreadEntityFadeManager.h"
#include "BreadSceneManager.h"
#include "BreadEntity.h"

namespace Bread
{
	/*
	*/
	void sEntityFadeInfo::setType(uint type)
	{
		m_type = type;
		if (FADE_IN == m_type)
		{
			m_alpha = 0.0f;
		}
		else if (FADE_OUT == m_type)
		{
			m_alpha = 1.0f;
		}
	}
	/*
	*/
	bool sEntityFadeInfo::update(float elapsedtime, BUID const& buid, float v)
	{
		bool ret = false;
		float s = elapsedtime * v;

		if (FADE_IN == m_type)
		{
			m_alpha += s;
			if (1.0f <= m_alpha)
			{
				m_alpha = 1.0f;
				ret = true;
			}
		}
		else if (FADE_OUT == m_type)
		{
			m_alpha -= s;
			if (0.0f >= m_alpha)
			{
				m_alpha = 0.0f;
				ret = true;
			}			
		}

		cEntity* entity = reinterpret_cast<cEntity*>(_getSceneMgr()->getEntity(buid));
		if (entity)
		{
			entity->setFadeAlpha(m_alpha);
		}
		else
		{
			ret = false;
		}

		return ret;
	}

	
	/*
	*/
	cEntityFadeManager::cEntityFadeManager()
	{
		m_v = 1.0f;
	}
	/*
	*/
	cEntityFadeManager::~cEntityFadeManager()
	{
	}
	/*
	*/
	void cEntityFadeManager::update(float elapsedtime)
	{
		for (begin(); !isEnd();)
		{
			if (getIterData().update(elapsedtime, getIterKey(), m_v))
			{
				removeNext();
			}
			else
			{
				next();
			}
		}
	}
	/*
	*/
	void cEntityFadeManager::addFadeIn(BUID const& buid)
	{
		add(buid, sEntityFadeInfo::FADE_IN);
	}
	/*
	*/
	void cEntityFadeManager::addFadeOut(BUID const& buid)
	{
		add(buid, sEntityFadeInfo::FADE_IN);
	}
	/*
	*/
	void cEntityFadeManager::add(BUID const& buid, uint type)
	{
		sEntityFadeInfo info;
		info.setType(type);
		insert(buid, info);
	}
}