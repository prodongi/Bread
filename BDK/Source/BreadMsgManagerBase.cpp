
#include "BreadMsgManagerBase.h"
#include <Windows.h>
#include <tchar.h>

namespace Bread
{
	/*
	*/
	cMsgManagerBase::cMsgManagerBase() : m_callBackNum(0), m_callBack(NULL)
	{
		m_msgList = new bucket_msg;
	}
	/*
	*/
	cMsgManagerBase::~cMsgManagerBase()
	{
		safeDelete(m_msgList);
		safeDeleteArray(m_callBack);
	}
	/*
	*/
	void cMsgManagerBase::initialize()
	{
		//initCallBack(callback number);
	}
	/*
	*/
	void cMsgManagerBase::initCallBack(uint callBackNum)
	{
		m_callBackNum = callBackNum;
		m_callBack = new MsgCallBack[m_callBackNum];
		ZeroMemory(m_callBack, sizeof (MsgCallBack) * m_callBackNum);

		initCallBackFunc();
	}
	/*
	*/
	void cMsgManagerBase::addBucket(sMsgInfo* msg)
	{
		m_msgList->pushBack(&msg);
	}
	/*
	*/
	void cMsgManagerBase::execute(sMsgInfo& msg)
	{
		if (!m_callBack[msg.m_type])
		{
			assert(m_callBack[msg.m_type]);
			msg.m_result = sMsgInfo::FAIL;
			return ;
		}
		if (m_callBackNum <= msg.m_type)
		{
			assert(0 && _T("invalid msg type"));
			msg.m_result = sMsgInfo::FAIL;
			return ;
		}

		msg.m_result = sMsgInfo::SUCCESS;
		m_callBack[msg.m_type](&msg);
	}
	/*
	*/
	void cMsgManagerBase::update(float /*elapsedtime*/)
	{
		for (m_msgList->begin(); !m_msgList->isEnd();)
		{
			sMsgInfo* info = m_msgList->getIterData();

			execute(*info);

			if (sMsgInfo::SUCCESS == info->m_result || sMsgInfo::FAIL == info->m_result)
			{
				m_msgList->erase();
				continue;
			}
			else if (sMsgInfo::NEXT == info->m_result)
			{
			}

			m_msgList->next();
		}
	}
}