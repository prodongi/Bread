
#include "BreadStateManager.h"
#include "BreadBaseState.h"
#include "BreadInline.h"
#include "BreadTrace.h"
#include <assert.h>

namespace Bread
{
	/*
	 *	
	 */	
	cStateManager::cStateManager() : m_curTime(0.0f)
	{
	}
	/*
	 *	
	 */
	cStateManager::~cStateManager()
	{
		terminate();
	}
	/*
	 *	
	 */
	void cStateManager::init()
	{
		m_curTime = 0.0f;
	}
	/*
	 *	
	 */
	void cStateManager::terminate()
	{
		m_curStateLayer.clear();
		safeClearMap<stateMap::iterator>(m_stateMap);
		allPopCmdQueue();
	}
	/*
	 *
	 */
	bool cStateManager::registerState(const char* stateName, iBaseState* state)
	{
		if (checkIsNull(stateName) || checkIsNull(state))
			return false;

		if (getStateClass(stateName))
		{
			assert(0 && "this state is existence");
			return false;
		}

		m_stateMap[stateName] = state;
		
		return true;
	}
	/*
	 *	
	 */
	bool cStateManager::isStateChange() const
	{
		return !m_cmdQueue.empty();
	}
	/*
	 *	
	 */
	const char* cStateManager::getCurState() const
	{
		if (m_curStateLayer.empty())
			return NULL;

		stateLayer::const_iterator it = m_curStateLayer.end() - 1;
		return it->c_str();
	}
	/*
	 *	
	 */
	iBaseState* cStateManager::getCurStateClass() const
	{
		const char* curStateName = getCurState();
		if (!curStateName)
		{
			return NULL;
		}

		return getStateClass(curStateName);
	}
	/*
	 *	
	 */
	iBaseState* cStateManager::getStateClass(const char* stateName) const
	{
		stateMap::const_iterator it = m_stateMap.find(stateName);
		if (it == m_stateMap.end())
		{
			return NULL;
		}

		return it->second;
	}
	/*
	 *	
	*/
	int cStateManager::getStackSize() const
	{
		return (int)m_curStateLayer.size();
	}
	/*
	*/
	void cStateManager::setState(const char* stateName, float delay/* = 0.0f*/, bool flush/* = false*/)
	{
		if (!stateName)
		{
			assert(0 && _T("stateName is NULL, failed setState"));
			return ;
		}

		if (flush)
			allPopCmdQueue();

		const char* oldStateName = getCurState();
		iBaseState* curState = getCurStateClass();
		if (curState)	curState->exit(stateName);
		m_curStateLayer.pop_back();
		m_curStateLayer.push_back(stateName);
		getCurStateClass()->enter(oldStateName);
	}
	/*
	 *	
	 */
	void cStateManager::changeState(const char* stateName, float delay/* = 0.0f*/, bool flush/* = false*/)
	{
		if (!stateName)
		{
			assert(0 && _T("stateName is NULL, failed changeState"));
			return ;
		}

		if (flush)
			allPopCmdQueue();
		
		sStateCmd stateCmd(CMD_CHANGE, stateName, m_curTime + delay);
		m_cmdQueue.push(stateCmd);
	}
	/*
	 *	
	 */
	void cStateManager::pushState(const char* stateName, float delay/* = 0.0f*/, bool flush/* = false*/)
	{
		if (!stateName)
		{
			assert(0 && _T("stateName is NULL, failed pushState"));
			return ;
		}
		if (flush)
			allPopCmdQueue();

		sStateCmd stateCmd(CMD_PUSH, stateName, m_curTime + delay);
		m_cmdQueue.push(stateCmd);
	}
	/*
	 *	
	 */
	void cStateManager::popState(int statesToPop, float delay/* = 0.0f*/, bool flush/* = false*/)
	{
		if (flush)
			allPopCmdQueue();

		sStateCmd stateCmd(CMD_POP, "", m_curTime + delay);
		for (int i = 0; i < statesToPop; ++i)
		{
			m_cmdQueue.push(stateCmd);
			// push한 첫 번째 상태만 0.0이 아닌 값을 갖도록 한다
			stateCmd.m_delay = 0.0f;
		}
	}
	/// 마지막 상태만 남기고 모두 뺀다.
	void cStateManager::popAllStates(float delay/* = 0.0f*/, bool flush/* = false*/)
	{
		if (flush)
			allPopCmdQueue();

		sStateCmd stateCmd(CMD_POP_ALL, "", m_curTime + delay);
		m_cmdQueue.push(stateCmd);
	}
	void cStateManager::update(float elapsedtime)
	{
		m_curTime += elapsedtime;

		sStateCmd stateCmd;
		while (!m_cmdQueue.empty())
		{
			if (m_cmdQueue.front().m_delay > m_curTime)
				break;

			stateCmd = m_cmdQueue.front();
			const char* oldStateName = getCurState();
			const char* queueStateName = stateCmd.m_stateName.c_str();
			iBaseState* curState = getCurStateClass();
			iBaseState* newState;
			if (CMD_PUSH == stateCmd.m_cmdType)
			{
				if(0 != strcmp(oldStateName, stateCmd.m_stateName.c_str()))
				{
					if (curState)	curState->override(queueStateName);
					m_curStateLayer.push_back(queueStateName);
					newState = getCurStateClass();
					if (newState)	newState->enter(oldStateName);

					trace(_T("enter state : %s"), queueStateName);
				}
			}
			else if (CMD_POP == stateCmd.m_cmdType)
			{
				if(m_curStateLayer.size() > 1)
				{
					if (curState) curState->exit(m_curStateLayer[m_curStateLayer.size() - 2].c_str());
					m_curStateLayer.pop_back();
					newState = getCurStateClass();
					if (newState)	newState->resume(oldStateName);

					trace(_T("exit state : %s"), oldStateName);
				}
			}
			else if (CMD_POP_ALL == stateCmd.m_cmdType)
			{
				while(m_curStateLayer.size() > 1)
				{
					const char* prevStateName = getCurState();
					curState = getCurStateClass();
					if (curState)	curState->exit(m_curStateLayer[m_curStateLayer.size() - 2].c_str());
					m_curStateLayer.pop_back();
					newState = getCurStateClass();
					if (newState)	newState->resume(prevStateName);

					trace(_T("exit state : %s"), prevStateName);
				}
			}
			else if (CMD_CHANGE == stateCmd.m_cmdType)
			{
				if(0 != strcmp(oldStateName, stateCmd.m_stateName.c_str()))
				{
					if (curState)	curState->exit(stateCmd.m_stateName.c_str());
					m_curStateLayer.pop_back();
					m_curStateLayer.push_back(stateCmd.m_stateName.c_str());
					newState = getCurStateClass();
					if (newState)	newState->enter(oldStateName);
				}
			}
			else
			{
				trace(_T("invalid state update"));
			}

			m_cmdQueue.pop();
		}

		// update state layer
		for(int i = (int)m_curStateLayer.size() - 1; i >= 0; --i)
		{
			iBaseState* state = getStateClass(m_curStateLayer[i].c_str());
			if (state)	
				state->update(elapsedtime);
		}
	}
	/*
	 *	
	 */
	void cStateManager::allPopCmdQueue()
	{
		while (!m_cmdQueue.empty())
			m_cmdQueue.pop();
	}
}