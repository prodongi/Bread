
#ifndef _BreadExceptionHandler_h_
#define _BreadExceptionHandler_h_

#include <Windows.h>

namespace Bread
{
	struct sExceptionMailInfo
	{
		sExceptionMailInfo() : m_isSend(false)	{}
		char m_recvName[32];
		char m_recvAddress[128];
		bool m_isSend;
	};
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
	 * \date 2010-04-05
	 *
	 * \author Administrator
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo Debugging Applications .NET Windows ÂüÁ¶
	 *
	 * \bug 
	 *
	 */
	class cExceptionHandler
	{
	public:
		cExceptionHandler();
		~cExceptionHandler();

		static LONG WINAPI exceptionFilterCallBack(PEXCEPTION_POINTERS exceptionInfo);
		static void setMailInfo(sExceptionMailInfo const& mailInfo);

	private:
		static LPCTSTR getFaultReason(PEXCEPTION_POINTERS exceptionPtr);
		static LPCTSTR getRegistString(PEXCEPTION_POINTERS exceptionPtr);
		static LPCTSTR getFirstStackTraceString(DWORD opt, PEXCEPTION_POINTERS exceptionPtr);
		static LPCTSTR getNextStackTraceString(DWORD opt, PEXCEPTION_POINTERS exceptionPtr);
		static void initSymEng();
		static LPCTSTR convertSimpleException ( DWORD dwExcept );
		/// @brief Helper function to isolate filling out the stack frame, which is CPU specific.
		static void fillInStackFrame (PCONTEXT pCtx);
		static LPCTSTR __stdcall internalGetStackTraceString ( DWORD dwOpts );

		static void writeException(LPCTSTR exceptionStr);
		static void writeMiniDump(PEXCEPTION_POINTERS exceptionPtr);
		static void sendMail();

		static DWORD WINAPI exceptionFilterCallBackFunc(LPVOID param);

	private:
		static LPTOP_LEVEL_EXCEPTION_FILTER m_oldFilter;
		static BOOL m_symEngInit;
		static sExceptionMailInfo m_mailInfo;
		static TCHAR* m_exceptionFileName;
	};

}












#endif