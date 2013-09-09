
#ifndef _BreadStateManager_h_
#define _BreadStateManager_h_

#include "BreadStdHeader.h"

namespace Bread
{
	class iBaseState;

	/**
	 * \ingroup Core
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
	 * \date 2009-07-25
	 *
	 * \author prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo ���¸� �����ϴ� Ŭ����
	 *
	 * \bug 
	 *
	 */
	class cStateManager
	{
	public:
		cStateManager();
		~cStateManager();

		void init();
		void terminate();

		bool registerState(const char* stateName, iBaseState* state);
		bool isStateChange() const;
		const char* getCurState() const;
		iBaseState* getCurStateClass() const;
		iBaseState* getStateClass(const char* stateName) const;
		int getStackSize() const;
		// cmdQueue�� ���� �ʰ� �ٷ� state�� ������
		void setState(const char* stateName, float delay = 0.0f, bool flush = false);
		void changeState(const char* stateName, float delay = 0.0f, bool flush = false);
		void pushState(const char* stateName, float delay = 0.0f, bool flush = false);
		/// ���� ���¸� ���ÿ��� ����. ������ ���´� �� �� ����.
		void popState(int statesToPop,	float delay = 0.0f, bool flush = false);
		/// ������ ���¸� ����� ��� ����.
		void popAllStates(float delay = 0.0f, bool flush = false);
		void update(float elapsedtime);

	private:
		void allPopCmdQueue();

	private:
		enum eStateCmdType
		{
			CMD_PUSH,
			CMD_CHANGE,
			CMD_POP,
			CMD_POP_ALL
		};

		struct sStateCmd
		{
			sStateCmd() {}
			sStateCmd(eStateCmdType cmdType, const char* stateName, float delay)
			{
				m_cmdType	= cmdType;
				m_stateName = stateName;
				m_delay		= delay;
			}
			eStateCmdType	m_cmdType;
			std::string     m_stateName;
			float			m_delay;
		};

		typedef std::vector<std::string>			stateLayer;
		typedef std::map<std::string, iBaseState*>	stateMap;
		typedef std::queue<sStateCmd>				cmdQueue;

		/// ���� ���� ���̾� ����Ʈ
		stateLayer	m_curStateLayer;
		/// ��ü ���� ����Ʈ
		stateMap	m_stateMap;
		cmdQueue	m_cmdQueue;
		float		m_curTime;
	};
}










#endif

















