
#include <windows.h>
#include "BreadProfileManager.h"
#include "BreadInternString.h"
#include "BreadStringHelper.h"
#include "BreadInline.h"
#include "BreadTimer.h"
#include "BreadMath.h"

namespace Bread
{
	callBackProfileRender cProfileNode::m_callBackRender = NULL;


	/*
	*/
	inline void profileGetTicks(__int64* ticks)
	{
		__asm
		{
			push edx;
			push ecx;
			mov ecx,ticks;
			_emit 0Fh
			_emit 31h
			mov [ecx],eax;
			mov [ecx+4],edx;
			pop ecx;
			pop edx;
		}
	}
	/*
	*/
	inline float profileGetTickRate()
	{
		static float cpuFrequency = -1.0f;

		if (cMath::equal(cpuFrequency, -1.0f))
		{
			__int64 curRate = 0;
			::QueryPerformanceFrequency((LARGE_INTEGER*)&curRate);
			cpuFrequency = (float)curRate;
		}
		return cpuFrequency;
	}

	int cProfileManager::m_renderDepth = 0;
	cProfileNode cProfileManager::m_root(cInternString::getSingleton()->getId(_T("root")), NULL);
	cProfileNode* cProfileManager::m_current = &cProfileManager::m_root;

	/*
	*/
	cProfileNode::cProfileNode(string_id id, cProfileNode* parent) : m_lastChild(NULL),
																	 m_firstChild(NULL),
																	 m_sibling(NULL),
																	 m_totalCall(0),
																	 m_totalTime(0.0f),
																	 m_startTime(0),
																	 m_recursionCounter(0)
	{
		m_id = id;
		m_parent = parent;
	}
	/*
	*/
	cProfileNode::~cProfileNode()
	{
		safeDelete(m_firstChild);
		safeDelete(m_sibling);
	}
	/*
	*/
	cProfileNode* cProfileNode::getSubNode(string_id id)
	{
		if (m_lastChild)
		{
			if (m_lastChild->m_id == id)
			{
				return m_lastChild;
			}
			else if (m_lastChild->m_sibling)
			{
				if (m_lastChild->m_sibling->m_id == id)
				{
					cProfileNode* retNode = m_lastChild->m_sibling;
					m_lastChild = m_lastChild->m_sibling;
					return retNode;
				}
			}
		}

		cProfileNode* node = new cProfileNode(id, this);
		if (!m_lastChild)
			m_firstChild = node;

		if (m_lastChild)
		{
			m_lastChild->m_sibling = node;
		}

		m_lastChild = node;
		
		return node;

	}
	/*
	*/
	void cProfileNode::call()
	{
		++m_totalCall;
		if (m_recursionCounter++ == 0)
			profileGetTicks(&m_startTime);
	}
	/*
	*/
	bool cProfileNode::callOff()
	{
		if (--m_recursionCounter == 0 && m_totalCall != 0)
		{
			__int64 time;
			profileGetTicks(&time);
			time -= m_startTime;
			m_totalTime += (float)time/profileGetTickRate();
		}
		return 0 == m_recursionCounter;
	}
	/*
	*/
	void cProfileNode::reset()
	{
		m_totalTime = 0.0f;
		m_totalCall = 0;

		if (m_firstChild)
			m_firstChild->reset();
		if (m_sibling)
			m_sibling->reset();

		if (m_firstChild)
			m_lastChild = m_firstChild;
	}
	/*
	*/
	void cProfileNode::print(int x, int y, int depth)
	{
		if (!m_callBackRender)
			return ;

		x += depth * 8;
		std_string id;
		_getInternString()->getStr(m_id, &id);
		TCHAR const* str = cStringHelper::getSingleton()->make(_T("%s, %2f, %d"), id.c_str(), m_totalTime, m_totalCall);
		str = str;
		(*m_callBackRender)(x, y, str);
	}

	/*
	*/
	cProfileIterator::cProfileIterator(cProfileNode* start)
	{
		m_parent = start;
		m_child = m_parent->getFirstChild();
	}
	/*
	*/
	void cProfileIterator::getFirst()
	{
		m_child = m_parent->getFirstChild();
	}
	/*
	*/
	void cProfileIterator::getNext()
	{
		m_child = m_child->getSibling();
	}
	/*
	*/
	bool cProfileIterator::isDone() const
	{
		return m_child == NULL;
	}
	/*
	*/
	bool cProfileIterator::enter(int depth)
	{
		for (int i = 0; i < depth; ++i)
		{
			m_parent = m_parent->getFirstChild();
			if (!m_parent)
				break;
		}
		return m_parent != NULL;
	}
	/*
	*/
	void cProfileIterator::render(int x, int y, int depth)
	{
		getFirst();
		render(x, y, depth, m_child);
	}
	/*
	*/
	void cProfileIterator::render(int x, int& y, int depth, cProfileNode* node)
	{
		y += 12;

		if (!node)
			return ;

		node->print(x, y, depth);

		cProfileNode* child = node->getFirstChild();
		if (child)
		{
			render(x, y, depth+1, child);
		}

		cProfileNode* sibling = node->getSibling();
		if (sibling)
		{
			render(x, y, depth, sibling);
		}
	}

	/*
	*/
	void cProfileManager::start(string_id id)
	{
		if (id != m_current->getId())
		{
			m_current = m_current->getSubNode(id);
		}
		m_current->call();
	}
	/*
	*/
	void cProfileManager::stop()
	{
		if (m_current->callOff())
			m_current = m_current->getParent();
	}
	/*
	*/
	void cProfileManager::reset()
	{
		m_root.reset();
	}
	/*
	*/
	void cProfileManager::render(int x, int y)
	{
		cProfileIterator iter(&m_root);
		iter.render(x, y, 0);
	}
}