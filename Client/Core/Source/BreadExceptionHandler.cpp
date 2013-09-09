
#include "BreadExceptionHandler.h"
#include "BreadClientSystemInfo.h"
#include "BreadCore.h"
#include "BreadUtils.h"
#include "BreadD3DSystemClassify.h"
#include "BreadStringHelper.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <ImageHlp.h>
#include <mapi.h>

#pragma comment(lib, "dbghelp")
//#pragma comment(linker, "/defaultlib:dbghelp.lib")

namespace Bread
{
	LPTOP_LEVEL_EXCEPTION_FILTER cExceptionHandler::m_oldFilter = NULL;
	BOOL cExceptionHandler::m_symEngInit = FALSE;
	sExceptionMailInfo cExceptionHandler::m_mailInfo = sExceptionMailInfo();
	TCHAR* cExceptionHandler::m_exceptionFileName = _T("minidump.rar");

	#define GSTSO_PARAMS    0x01
	#define GSTSO_MODULE    0x02
	#define GSTSO_SYMBOL    0x04
	#define GSTSO_SRCLINE   0x08

	#define MAX_SYM_SIZE  512
	#define BUFF_SIZE 2048
	#define SYM_BUFF_SIZE 1024

	// The static buffer returned by various functions. This buffer
	// allows data to be transferred without using the stack.
	static TCHAR g_szBuff [ BUFF_SIZE ] ;
	// The static symbol lookup buffer
	static BYTE g_stSymbol [ SYM_BUFF_SIZE ] ;
	// The static source file and line number structure
	static IMAGEHLP_LINE64 g_stLine ;
	// The stack frame used in walking the stack
	static STACKFRAME64 g_stFrame ;
	// The original version of this code changed the CONTEXT structure when
	// passed through the stack walking code.  Therefore, if the user
	// utilized the containing  EXCEPTION_POINTERS to write a mini dump, the
	// dump wasn't correct.  I now save off the CONTEXT as a global, much
	// like the stack frame.
	static CONTEXT g_stContext ;

	// String format constants.  To avoid doing tons of ANSI to UNICODE
	// conversions myself, I rely on wsprintf to do them.  In order to make
	// this file happy for ANSI compiles, I need keep the %S out of the
	// format strings.
	#ifdef UNICODE
	#define k_NAMEDISPFMT       _T ( "%S()+%04d byte(s)" )
	#define k_NAMEFMT           _T ( "%S " )
	#define k_FILELINEDISPFMT   _T ( "%S, line %04d+%04d byte(s)" )
	#define k_FILELINEFMT       _T ( "%S, line %04d" )
	#else
	#define k_NAMEDISPFMT       _T ( "%s()+%04d byte(s)" )
	#define k_NAMEFMT           _T ( "%s " )
	#define k_FILELINEDISPFMT   _T ( "%s, line %04d+%04d byte(s)" )
	#define k_FILELINEFMT       _T ( "%s, line %04d" )
	#endif

	#ifdef _WIN64
	#define k_PARAMFMTSTRING   _T ( " (0x%016X 0x%016X 0x%016X 0x%016X)" )
	#else
	#define k_PARAMFMTSTRING   _T ( " (0x%08X 0x%08X 0x%08X 0x%08X)" )
	#endif

	// Define the machine type.
	#ifdef _X86_
	#define CH_MACHINE IMAGE_FILE_MACHINE_I386
	#elif _AMD64_
	#define CH_MACHINE IMAGE_FILE_MACHINE_AMD64
	#elif _IA64_
	#define CH_MACHINE IMAGE_FILE_MACHINE_IA64
	#else
	#pragma FORCE COMPILE ABORT!
	#endif


	/*
	 *	
	 */
	BOOL __stdcall CH_ReadProcessMemory (HANDLE /*handle*/,
										 DWORD64 qwBaseAddress,
										 PVOID lpBuffer,
										 DWORD nSize,
										 LPDWORD lpNumberOfBytesRead)
	{
		return ( ReadProcessMemory ( GetCurrentProcess ( )  ,
			(LPCVOID)qwBaseAddress ,
			lpBuffer               ,
			nSize                  ,
			lpNumberOfBytesRead     ) ) ;
	}




	/*
	*	
	*/
	cExceptionHandler::cExceptionHandler()
	{
		m_oldFilter = SetUnhandledExceptionFilter(exceptionFilterCallBack);
	}
	/*
	*	
	*/
	cExceptionHandler::~cExceptionHandler()
	{
		SetUnhandledExceptionFilter(m_oldFilter);
	}
	/*
	*	
	*/
	void cExceptionHandler::setMailInfo(sExceptionMailInfo const& mailInfo)
	{
		m_mailInfo.m_isSend = mailInfo.m_isSend;
		strcpy(m_mailInfo.m_recvName, mailInfo.m_recvName);
		strcpy(m_mailInfo.m_recvAddress, mailInfo.m_recvAddress);
	}
	/*
	*	
	*/
	LONG WINAPI cExceptionHandler::exceptionFilterCallBack(PEXCEPTION_POINTERS exceptionPtr)
	{
		/// 스택 오버플로우 됬을 때는 exceptionFilterCallBackFunc호출도 깨질 수 있기 때문에, 쓰레드를 생성해서 호출함,
		/// 새로 생성된 쓰레드는 스택이 초기화 상태이기 때문에 깨지지 않는다. 왜 stack size가 102400인지는 모르겠음.
		if (EXCEPTION_STACK_OVERFLOW == exceptionPtr->ExceptionRecord->ExceptionCode)
		{
			HANDLE hThread = CreateThread(NULL, 102400, exceptionFilterCallBackFunc, exceptionPtr, 0, NULL);
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		else
		{
			exceptionFilterCallBackFunc(exceptionPtr);
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}
	/*
	*/
	DWORD WINAPI cExceptionHandler::exceptionFilterCallBackFunc(LPVOID param)
	{
		PEXCEPTION_POINTERS exceptionPtr = reinterpret_cast<PEXCEPTION_POINTERS>(param);

		std_string buffer;
		TCHAR const* szBuff = NULL;

		szBuff = getFaultReason(exceptionPtr);
		if (szBuff)
		{
			buffer += szBuff;
			buffer += _T("\n\n");
		}

		szBuff = getRegistString(exceptionPtr);
		if (szBuff)
		{
			buffer += szBuff;
			buffer += _T("\n\n");
		}

		DWORD dwOpts = /*GSTSO_PARAMS | */GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;
		szBuff = getFirstStackTraceString(dwOpts, exceptionPtr);

		while (szBuff)
		{
			buffer += szBuff;
			buffer += _T("\n");
			szBuff = getNextStackTraceString(dwOpts, exceptionPtr);
		}

		writeException(buffer.c_str());
		writeMiniDump(exceptionPtr);
		sendMail();

		MessageBox(HWND_DESKTOP, _T("Check the exception.log"), _T("Exception"), MB_OK);

		return 0;
	}
	/*
	*	
	*/
	LPCTSTR cExceptionHandler::getFaultReason(PEXCEPTION_POINTERS exceptionPtr)
	{
		// The variable that holds the return value
		LPCTSTR szRet ;

		__try
		{
			DWORD exceptionCode = exceptionPtr->ExceptionRecord->ExceptionCode;
			PVOID exceptionAddress = exceptionPtr->ExceptionRecord->ExceptionAddress;

			// Initialize the symbol engine in case it isn't initialized.
			initSymEng ( ) ;

			// The current position in the buffer
			int iCurr = 0 ;
			// A temporary value holder. This holder keeps the stack usage
			// to a minimum.
			DWORD64 dwTemp ;
			iCurr += _stprintf(g_szBuff, _T("[Project]"));
			iCurr += _stprintf(g_szBuff + iCurr , _T ( " caused an " ) ) ;

			dwTemp = (DWORD_PTR)convertSimpleException(exceptionCode);

			if (NULL != dwTemp)
			{
				iCurr += _stprintf(g_szBuff + iCurr, _T("%I64d"), dwTemp) ;
			}
			else
			{
				iCurr += FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
					GetModuleHandle (_T("NTDLL.DLL")) ,
					exceptionCode, 0, g_szBuff + iCurr, BUFF_SIZE, 0);
			}

			assert ( iCurr < ( BUFF_SIZE - MAX_PATH ) ) ;
			iCurr += _stprintf(g_szBuff + iCurr, _T (" in module\n") ) ;

			dwTemp = SymGetModuleBase64(GetCurrentProcess(), (DWORD64)exceptionAddress);
			assert(NULL != dwTemp) ;

			if (NULL == dwTemp)
			{
				iCurr += _stprintf(g_szBuff + iCurr, _T(" <UNKNOWN> "));
			}
			else
			{
				iCurr += _stprintf(g_szBuff + iCurr, _T("[Project]"));
			}

#ifdef _WIN64
			iCurr += _stprintf ( g_szBuff + iCurr, _T ( " at %016X" )   ,
				exceptionPtr->ExceptionRecord->ExceptionAddress);
#else
			iCurr += _stprintf (g_szBuff + iCurr, _T (" at %04X:%08X" ),
				exceptionPtr->ContextRecord->SegCs, exceptionAddress);
#endif

			assert(iCurr < ( BUFF_SIZE - 200 )) ;

			// Start looking up the exception address.
			PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&g_stSymbol ;
			ZeroMemory ( pSym , SYM_BUFF_SIZE ) ;
			pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
			pSym->MaxNameLength = SYM_BUFF_SIZE - sizeof ( IMAGEHLP_SYMBOL64 ) ;

			DWORD64 dwDisp ;
			if ( TRUE == SymGetSymFromAddr64 (GetCurrentProcess (),
				(DWORD64)exceptionAddress, &dwDisp, pSym))
			{
				iCurr += _stprintf(g_szBuff + iCurr , _T( ", " )) ;

				// Copy no more of the symbol information than there's
				// room for.  Remember, symbols names are ANSI!
				int iLen = lstrlenA ( pSym->Name ) ;
				// Make sure there's enough room for the longest symbol
				// and the displacement.
				if ( iLen > ( ( BUFF_SIZE - iCurr) - ( MAX_SYM_SIZE + 50 )  ) )
				{
#ifdef UNICODE
					// Get some room on the stack to convert the string.
					TCHAR * pWideName = (TCHAR*)_alloca ( iLen + 1 ) ;
					_getStringHelper()->m2w(pSym->Name, pWideName, iLen + 1);
					lstrcpyn ( g_szBuff + iCurr, pWideName, BUFF_SIZE - iCurr - 1  ) ;
#else
					lstrcpyn ( g_szBuff + iCurr, pSym->Name, BUFF_SIZE - iCurr - 1  ) ;
#endif  // UNICODE
					// Gotta leave now
					szRet = g_szBuff ;
					__leave ;
				}
				else
				{
					if ( dwDisp > 0 )
					{
						iCurr += _stprintf ( g_szBuff + iCurr, k_NAMEDISPFMT, pSym->Name, dwDisp);
					}
					else
					{
						iCurr += _stprintf ( g_szBuff + iCurr, k_NAMEFMT, pSym->Name);
					}
				}
			}
			else
			{
				// If the symbol wasn't found, the source and line won't
				// be found either, so leave now.
				szRet = g_szBuff ;
				__leave ;
			}

			assert ( iCurr < ( BUFF_SIZE - 200 ) ) ;

			// Look up the source file and line number.
			ZeroMemory ( &g_stLine , sizeof ( IMAGEHLP_LINE64 ) ) ;
			g_stLine.SizeOfStruct = sizeof ( IMAGEHLP_LINE64 ) ;

			DWORD dwLineDisp ;
			if ( TRUE == SymGetLineFromAddr64 (GetCurrentProcess (), (DWORD64)exceptionAddress,
				&dwLineDisp, &g_stLine) )
			{
				iCurr += _stprintf(g_szBuff + iCurr , _T ( ",\n" ));

				// Copy no more of the source file and line number
				// information than there's room for.
				int iLen = lstrlenA ( g_stLine.FileName ) ;
				if ( iLen > ( BUFF_SIZE - iCurr - MAX_PATH - 50       ) )
				{
#ifdef UNICODE
					// Get some room on the stack to convert the string.
					TCHAR * pWideName = (TCHAR*)_alloca ( iLen + 1 ) ;
					_getStringHelper()->m2w(g_stLine.FileName, pWideName, iLen+1);
					lstrcpyn ( g_szBuff + iCurr, pWideName, BUFF_SIZE - iCurr - 1) ;
#else
					lstrcpyn ( g_szBuff + iCurr, g_stLine.FileName, BUFF_SIZE - iCurr - 1) ;

#endif  // UNICODE
					// Gotta leave now
					szRet = g_szBuff ;
					__leave ;
				}
				else
				{
					if ( dwLineDisp > 0 )
					{
						iCurr += _stprintf(g_szBuff + iCurr, k_FILELINEDISPFMT, g_stLine.FileName,
										   g_stLine.LineNumber, dwLineDisp);
					}
					else
					{
						iCurr += _stprintf(g_szBuff + iCurr, k_FILELINEFMT, g_stLine.FileName, g_stLine.LineNumber) ;
					}
				}
			}
			szRet = g_szBuff ;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			assert ( !"Crashed in GetFaultReason" ) ;
			szRet = NULL ;
		}
		return ( szRet ) ;
	}
	/*
	*	
	*/
	LPCTSTR cExceptionHandler::getRegistString(PEXCEPTION_POINTERS exceptionPtr)
	{
#ifdef _X86_
		// This call puts 48 bytes on the stack, which could be a problem if
		// the stack is blown.
		_stprintf(g_szBuff ,
			_T ("EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X  ESI=%08X\n")\
			_T ("EDI=%08X  EBP=%08X  ESP=%08X  EIP=%08X  FLG=%08X\n")\
			_T ("CS=%04X   DS=%04X  SS=%04X  ES=%04X   ")\
			_T ("FS=%04X  GS=%04X" ) ,
			exceptionPtr->ContextRecord->Eax      ,
			exceptionPtr->ContextRecord->Ebx      ,
			exceptionPtr->ContextRecord->Ecx      ,
			exceptionPtr->ContextRecord->Edx      ,
			exceptionPtr->ContextRecord->Esi      ,
			exceptionPtr->ContextRecord->Edi      ,
			exceptionPtr->ContextRecord->Ebp      ,
			exceptionPtr->ContextRecord->Esp      ,
			exceptionPtr->ContextRecord->Eip      ,
			exceptionPtr->ContextRecord->EFlags   ,
			exceptionPtr->ContextRecord->SegCs    ,
			exceptionPtr->ContextRecord->SegDs    ,
			exceptionPtr->ContextRecord->SegSs    ,
			exceptionPtr->ContextRecord->SegEs    ,
			exceptionPtr->ContextRecord->SegFs    ,
			exceptionPtr->ContextRecord->SegGs     ) ;
#elif _AMD64_
		_stprintf ( g_szBuff ,
			_T ("RAX=%016X  RBX=%016X  RCX=%016X  RDX=%016X  RSI=%016X\n")\
			_T ("RDI=%016X  RBP=%016X  RSP=%016X  RIP=%016X  FLG=%016X\n")\
			_T (" R8=%016X   R9=%016X  R10=%016X  R11=%016X  R12=%016X\n")\
			_T ("R13=%016X  R14=%016X  R15=%016X" ) ,
			exceptionPtr->ContextRecord->Rax      ,
			exceptionPtr->ContextRecord->Rbx      ,
			exceptionPtr->ContextRecord->Rcx      ,
			exceptionPtr->ContextRecord->Rdx      ,
			exceptionPtr->ContextRecord->Rsi      ,
			exceptionPtr->ContextRecord->Rdi      ,
			exceptionPtr->ContextRecord->Rbp      ,
			exceptionPtr->ContextRecord->Rsp      ,
			exceptionPtr->ContextRecord->Rip      ,
			exceptionPtr->ContextRecord->EFlags   ,
			exceptionPtr->ContextRecord->R8       ,
			exceptionPtr->ContextRecord->R9       ,
			exceptionPtr->ContextRecord->R10      ,
			exceptionPtr->ContextRecord->R11      ,
			exceptionPtr->ContextRecord->R12      ,
			exceptionPtr->ContextRecord->R13      ,
			exceptionPtr->ContextRecord->R14      ,
			exceptionPtr->ContextRecord->R15       ) ;
#elif _IA64_
#pragma message ( "IA64 NOT DEFINED!!" )
#pragma FORCE COMPILATION ABORT!
#else
#pragma message ( "CPU NOT DEFINED!!" )
#pragma FORCE COMPILATION ABORT!
#endif

		return g_szBuff;
	}
	/*
	*	
	*/
	LPCTSTR cExceptionHandler::getFirstStackTraceString(DWORD opt, PEXCEPTION_POINTERS exceptionPtr)
	{
		// Get the stack frame filled in.
		fillInStackFrame (exceptionPtr->ContextRecord ) ;

		// Copy over the exception pointers fields so I don't corrupt the
		// real one.
		g_stContext = *(exceptionPtr->ContextRecord) ;

		return internalGetStackTraceString (opt);
	}
	/*
	*	
	*/
	LPCTSTR cExceptionHandler::getNextStackTraceString(DWORD opt, PEXCEPTION_POINTERS /*exceptionPtr*/)
	{
		// All error checking is in InternalGetStackTraceString.
		// Assume that GetFirstStackTraceString has already initialized the
		// stack frame information.
		return internalGetStackTraceString(opt) ;
	}



	void cExceptionHandler::initSymEng()
	{
		if (FALSE == m_symEngInit)
		{
			// Set up the symbol engine.
			// Retrieves the current option mask.
			DWORD dwOpts = SymGetOptions() ;

			// Turn on line loading.
			SymSetOptions(dwOpts | SYMOPT_LOAD_LINES) ;

			// Force the invade process flag on.
			// Initializes the symbol handler for a process.
			BOOL bRet = SymInitialize (GetCurrentProcess (), NULL, TRUE);
			assert ( TRUE == bRet ) ;
			m_symEngInit = bRet ;
		}
	}
	/*
	*	
	*/
	LPCTSTR cExceptionHandler::convertSimpleException ( DWORD dwExcept )
	{
		switch ( dwExcept )
		{
		case EXCEPTION_ACCESS_VIOLATION:			return _T ( "EXCEPTION_ACCESS_VIOLATION" );
		case EXCEPTION_DATATYPE_MISALIGNMENT:		return _T ( "EXCEPTION_DATATYPE_MISALIGNMENT" );
		case EXCEPTION_BREAKPOINT:					return _T ( "EXCEPTION_BREAKPOINT" );
		case EXCEPTION_SINGLE_STEP:					return _T ( "EXCEPTION_SINGLE_STEP" );
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		return _T ( "EXCEPTION_ARRAY_BOUNDS_EXCEEDED" );
		case EXCEPTION_FLT_DENORMAL_OPERAND:		return _T ( "EXCEPTION_FLT_DENORMAL_OPERAND" );
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:			return _T ( "EXCEPTION_FLT_DIVIDE_BY_ZERO" );
		case EXCEPTION_FLT_INEXACT_RESULT:			return _T ( "EXCEPTION_FLT_INEXACT_RESULT" );
		case EXCEPTION_FLT_INVALID_OPERATION:		return _T ( "EXCEPTION_FLT_INVALID_OPERATION" );
		case EXCEPTION_FLT_OVERFLOW:				return _T ( "EXCEPTION_FLT_OVERFLOW" );
		case EXCEPTION_FLT_STACK_CHECK:				return _T ( "EXCEPTION_FLT_STACK_CHECK" );
		case EXCEPTION_FLT_UNDERFLOW:				return _T ( "EXCEPTION_FLT_UNDERFLOW" );
		case EXCEPTION_INT_DIVIDE_BY_ZERO:			return _T ( "EXCEPTION_INT_DIVIDE_BY_ZERO" );
		case EXCEPTION_INT_OVERFLOW:				return _T ( "EXCEPTION_INT_OVERFLOW" );
		case EXCEPTION_PRIV_INSTRUCTION:			return _T ( "EXCEPTION_PRIV_INSTRUCTION" );
		case EXCEPTION_IN_PAGE_ERROR:				return _T ( "EXCEPTION_IN_PAGE_ERROR" );
		case EXCEPTION_ILLEGAL_INSTRUCTION:			return _T ( "EXCEPTION_ILLEGAL_INSTRUCTION" );
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:	return _T ( "EXCEPTION_NONCONTINUABLE_EXCEPTION" );
		case EXCEPTION_STACK_OVERFLOW:				return _T ( "EXCEPTION_STACK_OVERFLOW" );
		case EXCEPTION_INVALID_DISPOSITION:			return _T ( "EXCEPTION_INVALID_DISPOSITION" );
		case EXCEPTION_GUARD_PAGE:					return _T ( "EXCEPTION_GUARD_PAGE" );
		case EXCEPTION_INVALID_HANDLE:				return _T ( "EXCEPTION_INVALID_HANDLE" );
		case 0xE06D7363:							return _T ( "Microsoft C++ Exception" );
		default:									return ( NULL ) ;
		}
	}
	/*
	*	
	*/
	void cExceptionHandler::fillInStackFrame (PCONTEXT pCtx)
	{
		// Initialize the STACKFRAME structure.
		ZeroMemory ( &g_stFrame , sizeof ( STACKFRAME64 ) ) ;

#ifdef _X86_
		g_stFrame.AddrPC.Offset       = pCtx->Eip    ;
		g_stFrame.AddrPC.Mode         = AddrModeFlat ;
		g_stFrame.AddrStack.Offset    = pCtx->Esp    ;
		g_stFrame.AddrStack.Mode      = AddrModeFlat ;
		g_stFrame.AddrFrame.Offset    = pCtx->Ebp    ;
		g_stFrame.AddrFrame.Mode      = AddrModeFlat ;
#elif  _AMD64_
		g_stFrame.AddrPC.Offset       = pCtx->Rip    ;
		g_stFrame.AddrPC.Mode         = AddrModeFlat ;
		g_stFrame.AddrStack.Offset    = pCtx->Rsp    ;
		g_stFrame.AddrStack.Mode      = AddrModeFlat ;
		g_stFrame.AddrFrame.Offset    = pCtx->Rbp    ;
		g_stFrame.AddrFrame.Mode      = AddrModeFlat ;
#elif  _IA64_
#pragma message ( "IA64 NOT DEFINED!!" )
#pragma FORCE COMPILATION ABORT!
#else
#pragma message ( "CPU NOT DEFINED!!" )
#pragma FORCE COMPILATION ABORT!
#endif
	}

	// The internal function that does all the stack walking
	LPCTSTR __stdcall cExceptionHandler::internalGetStackTraceString ( DWORD dwOpts )
	{

		// The value that is returned
		LPCTSTR szRet ;
		// The module base address. I look this up right after the stack
		// walk to ensure that the module is valid.
		DWORD64 dwModBase ;

		__try
		{
			// Initialize the symbol engine in case it isn't initialized.
			initSymEng ( ) ;

			// Note:  If the source file and line number functions are used,
			//        StackWalk can cause an access violation.
			BOOL bSWRet = StackWalk64 (CH_MACHINE                         ,
				GetCurrentProcess ( )              ,
				GetCurrentThread ( )               ,
				&g_stFrame                         ,
				&g_stContext                       ,
				CH_ReadProcessMemory               ,
				SymFunctionTableAccess64           ,
				SymGetModuleBase64                 ,
				NULL                               );
			if ( ( FALSE == bSWRet ) || ( 0 == g_stFrame.AddrFrame.Offset ))
			{
				szRet = NULL ;
				__leave ;
			}

			// Before I get too carried away and start calculating
			// everything, I need to double-check that the address returned
			// by StackWalk really exists. I've seen cases in which
			// StackWalk returns TRUE but the address doesn't belong to
			// a module in the process.
			dwModBase = SymGetModuleBase64(GetCurrentProcess(), g_stFrame.AddrPC.Offset) ;
			if ( 0 == dwModBase )
			{
				szRet = NULL ;
				__leave ;
			}

			int iCurr = 0 ;

			// At a minimum, put in the address.
#ifdef _WIN64
			iCurr += _stprintf ( g_szBuff + iCurr, _T ( "0x%016X" ), g_stFrame.AddrPC.Offset) ;
#else
			iCurr += _stprintf ( g_szBuff + iCurr, _T ( "%04X:%08X" ), g_stContext.SegCs, g_stFrame.AddrPC.Offset) ;
#endif

			// Output the parameters?
			if ( GSTSO_PARAMS == ( dwOpts & GSTSO_PARAMS ) )
			{
				iCurr += _stprintf ( g_szBuff + iCurr          ,
					k_PARAMFMTSTRING          ,
					g_stFrame.Params[ 0 ]     ,
					g_stFrame.Params[ 1 ]     ,
					g_stFrame.Params[ 2 ]     ,
					g_stFrame.Params[ 3 ]      ) ;
			}
			// Output the module name.
			if ( GSTSO_MODULE == ( dwOpts & GSTSO_MODULE ) )
			{
				iCurr += _stprintf ( g_szBuff + iCurr  , _T ( " " ) ) ;

				assert(iCurr < (BUFF_SIZE - MAX_PATH));
				iCurr += _stprintf(g_szBuff + iCurr, _T("[Project]"));
			}

			assert(iCurr < (BUFF_SIZE - MAX_PATH)) ;
			DWORD64 dwDisp ;

			// Output the symbol name?
			if ( GSTSO_SYMBOL == ( dwOpts & GSTSO_SYMBOL ) )
			{
				// Start looking up the exception address.
				PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&g_stSymbol ;
				ZeroMemory ( pSym , SYM_BUFF_SIZE ) ;
				pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
				pSym->MaxNameLength = SYM_BUFF_SIZE -
					sizeof ( IMAGEHLP_SYMBOL64 ) ;
				pSym->Address = g_stFrame.AddrPC.Offset ;

				if ( TRUE ==
					SymGetSymFromAddr64 ( GetCurrentProcess ( )      ,
					g_stFrame.AddrPC.Offset    ,
					&dwDisp                    ,
					pSym                        ) )
				{
					if ( dwOpts & ~GSTSO_SYMBOL )
					{
						iCurr += _stprintf(g_szBuff + iCurr, _T ( ", " ));
					}

					// Copy no more symbol information than there's room
					// for.  Symbols are ANSI
					int iLen = lstrlenA ( pSym->Name ) ;
					if ( iLen > ( BUFF_SIZE - iCurr -
						( MAX_SYM_SIZE + 50 ) ) )
					{
#ifdef UNICODE
						// Get some room on the stack to convert the string.
						wchar_t * pWideName = (TCHAR*)_alloca ( iLen + 1 ) ;
						_getStringHelper()->m2w(pSym->Name, pWideName, iLen+1);
						lstrcpyn ( g_szBuff + iCurr, pWideName, BUFF_SIZE - iCurr - 1) ;
#else
						lstrcpyn ( g_szBuff + iCurr, pSym->Name, BUFF_SIZE - iCurr - 1) ;

#endif  // UNICODE
						// Gotta leave now
						szRet = g_szBuff ;
						__leave ;
					}
					else
					{
						if ( dwDisp > 0 )
						{
							iCurr += _stprintf(g_szBuff + iCurr, k_NAMEDISPFMT, pSym->Name, dwDisp) ;
						}
						else
						{
							iCurr += _stprintf(g_szBuff + iCurr, k_NAMEFMT, pSym->Name);
						}
					}
				}
				else
				{
					// If the symbol wasn't found, the source file and line
					// number won't be found either, so leave now.
					szRet = g_szBuff ;
					__leave ;
				}

			}

			assert ( iCurr < ( BUFF_SIZE - MAX_PATH ) ) ;

			// Output the source file and line number information?
			if ( GSTSO_SRCLINE == ( dwOpts & GSTSO_SRCLINE ) )
			{
				ZeroMemory ( &g_stLine , sizeof ( IMAGEHLP_LINE64 ) ) ;
				g_stLine.SizeOfStruct = sizeof ( IMAGEHLP_LINE64 ) ;

				DWORD dwLineDisp ;
				if ( TRUE == SymGetLineFromAddr64 ( GetCurrentProcess ( )  ,
					g_stFrame.AddrPC.Offset,
					&dwLineDisp            ,
					&g_stLine             ))
				{
					if ( dwOpts & ~GSTSO_SRCLINE )
					{
						iCurr += _stprintf(g_szBuff + iCurr , _T ( ", " ));
					}

					// Copy no more of the source file and line number
					// information than there's room for.
					int iLen = lstrlenA ( g_stLine.FileName ) ;
					if ( iLen > ( BUFF_SIZE - iCurr -
						( MAX_PATH + 50     ) ) )
					{
#ifdef UNICODE
						// Get some room on the stack to convert the string.
						TCHAR * pWideName = (TCHAR*)_alloca ( iLen + 1 ) ;
						_getStringHelper()->m2w(g_stLine.FileName, pWideName, iLen+1);
						lstrcpyn ( g_szBuff + iCurr, pWideName, BUFF_SIZE - iCurr - 1);
#else
						lstrcpyn ( g_szBuff + iCurr, g_stLine.FileName, BUFF_SIZE - iCurr - 1) ;

#endif
						// Gotta leave now
						szRet = g_szBuff ;
						__leave ;
					}
					else
					{
						if ( dwLineDisp > 0 )
						{
							iCurr += _stprintf(g_szBuff + iCurr, k_FILELINEDISPFMT,
											   g_stLine.FileName, g_stLine.LineNumber, dwLineDisp);
						}
						else
						{
							iCurr += _stprintf(g_szBuff + iCurr, k_FILELINEFMT,
											   g_stLine.FileName, g_stLine.LineNumber) ;
						}
					}
				}
			}

			szRet = g_szBuff ;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			assert ( !"Crashed in InternalGetStackTraceString" ) ;
			szRet = NULL ;
		}
		return ( szRet ) ;
	}
	/*
	*	
	*/
	void cExceptionHandler::writeException(LPCTSTR exceptionStr)
	{
		cClientSystemInfo sysInfo;
		std::string strSysInfo;
		sysInfo.getSystemInfo(strSysInfo, cSystemInfo::SI_ALL);

		std::ofstream fout;

		std_string filename;
		filename += _getDirectoryInfo()->getDirectory();
		filename += _T("\\");
		filename += _T("exception.inf");


		fout.open(filename.c_str(), std::ios::out);
		if (!fout.is_open())
		{
			assert(0 && _T("failed write exception file"));
			return ;
		}

		size_t len = _tcslen(exceptionStr);
		assert(len && _T("invalid write exception string size"));
		char* str = new char[len+1];
		_getStringHelper()->w2m(exceptionStr, str, len+1);

		char* temp;

		// time
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		char strTime[MAX_PATH];
		sprintf(strTime, "\n%04d-%02d-%02d(yymmdd), \t%02d:%02d:%02d(hhmmss)\n\n", 
				systemTime.wYear, systemTime.wMonth, systemTime.wDay,
				systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		fout.write(strTime, strlen(strTime));

		// system info
		temp = "########################################\n";	
		fout.write(temp, strlen(temp));
		temp = "#### system ############################\n";
		fout.write(temp, strlen(temp));
		temp = "########################################";	
		fout.write(temp, strlen(temp));

		fout.write(strSysInfo.c_str(), strSysInfo.length());
		temp = "\n";			
		fout.write(temp, strlen(temp));

		// exception info
		temp = "########################################\n";	
		fout.write(temp, strlen(temp));
		temp = "#### exception #########################\n";
		fout.write(temp, strlen(temp));
		temp = "########################################\n\n";	
		fout.write(temp, strlen(temp));

		fout.write(str, len);
		temp = "\n";			
		fout.write(temp, strlen(temp));

		fout.close();
	}
	/*
	 *	
	 */
	void cExceptionHandler::writeMiniDump(PEXCEPTION_POINTERS exceptionPtr)
	{
		typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
			HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
			CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
			CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
			CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
			);

		// load dll
		TCHAR dbgHelpPath[MAX_PATH];
		HMODULE hDll = NULL;
		if (GetModuleFileName(NULL, dbgHelpPath, MAX_PATH))
		{
			TCHAR* l = _tcsrchr(dbgHelpPath, '\\');
			if (l)
			{
				_tcscpy(l + 1, _T("dbghelp.dll"));
				hDll = ::LoadLibrary(dbgHelpPath);
			}
		}
		if (NULL == hDll)
		{
			hDll = LoadLibrary(_T("dbghelp.dll"));
		}
		if (NULL == hDll)
		{
			assert(0 && _T("failed load dbghelp.dll"));
			return ;
		}

		// find MiniDumpWriteDump
		MINIDUMPWRITEDUMP fnMiniDumpWriteDump = (MINIDUMPWRITEDUMP)GetProcAddress(hDll, "MiniDumpWriteDump");
		if (NULL == fnMiniDumpWriteDump)
		{
			assert(0 && _T("dbghelp.dll too old"));
			::FreeLibrary(hDll);
			return ;
		}

		std_string filename;
		filename += _getDirectoryInfo()->getDirectory();
		filename += _T("\\");
		filename += m_exceptionFileName;

		MINIDUMP_TYPE type = MiniDumpNormal;//MiniDumpWithDataSegs;//MiniDumpWithHandleData;

		HANDLE file = CreateFile(filename.c_str(),
								 GENERIC_READ | GENERIC_WRITE,
								 0,
								 NULL,
								 CREATE_ALWAYS,
								 FILE_ATTRIBUTE_NORMAL,
								 NULL);
		if (INVALID_HANDLE_VALUE == file)
		{
			assert(0 && _T("failed create dump file"));
			::FreeLibrary(hDll);
			return ;
		}
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = exceptionPtr;
		mdei.ClientPointers = FALSE;

		BOOL ret = fnMiniDumpWriteDump(GetCurrentProcess(),
									 GetCurrentProcessId(),
									 file,
									 type,
									 &mdei,
									 NULL,
									 NULL);
		CloseHandle(file);
		if (!ret)
		{
			assert(0 && _T("failed MiniDumpWriteDump Call"));
		}
		::FreeLibrary(hDll);
	}
	/*
	 *	
	 */
	void cExceptionHandler::sendMail()
	{
		if (!m_mailInfo.m_isSend)
			return ;

		HINSTANCE hMail = NULL;
		hMail = ::LoadLibrary(_T("MAPI32.DLL"));
		if (NULL == hMail)
		{
			assert(0 && _T("failed load mapi dll"));
			return ;
		}

		typedef ULONG (WINAPI *PMAPISENDMAIL)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
		PMAPISENDMAIL lpfnSendMail = (PMAPISENDMAIL)GetProcAddress(hMail, "MAPISendMail");
		if (NULL == lpfnSendMail)
		{
			assert(0 && _T("Invalid mapi dll"));
			::FreeLibrary(hMail);
			return ;
		}

		char recvAddress[128];
		sprintf(recvAddress, "SMTP:%s", m_mailInfo.m_recvAddress);

		MapiRecipDesc rdesc;
		ZeroMemory(&rdesc, sizeof (MapiRecipDesc));
		rdesc.ulRecipClass = MAPI_TO;
		rdesc.lpszName = m_mailInfo.m_recvName;
		rdesc.lpszAddress = recvAddress;

		std_string filename;
		filename += _getDirectoryInfo()->getDirectory();
		filename += _T("\\");
		filename += m_exceptionFileName;
		char exceptionPathName[MAX_PATH];
		_getStringHelper()->w2m(filename.c_str(), exceptionPathName, MAX_PATH);
		char exceptionFileName[MAX_PATH];
		_getStringHelper()->w2m(m_exceptionFileName, exceptionFileName, MAX_PATH);

		MapiFileDesc fdesc;
		ZeroMemory(&fdesc, sizeof (MapiFileDesc));
		fdesc.lpszPathName = exceptionPathName;
		fdesc.lpszFileName = exceptionFileName;

		MapiMessage message;
		ZeroMemory(&message, sizeof (MapiMessage));
		message.nRecipCount = 1;
		message.lpRecips = &rdesc;
		message.lpFiles = &fdesc;
		message.lpszSubject = "Exception has occurred";
		message.lpszNoteText = "Exception has occurred";

		ULONG ret = lpfnSendMail(0, (ULONG_PTR)_getD3DSystem()->getHwnd(), &message, MAPI_LOGON_UI | MAPI_DIALOG, 0);
		if (SUCCESS_SUCCESS != ret && MAPI_E_USER_ABORT != ret)
		{
			assert(0 && _T("failed send email"));
			::FreeLibrary(hMail);
			return ;
		}

		::FreeLibrary(hMail);
		MessageBox(HWND_DESKTOP, _T("send mail"), _T("ok"), MB_OK);
	}
}