
#include "BreadInputLocaleInfo.h"
#include "BreadCore.h"
#include "BreadD3DFont.h"
#include "BreadStringHelper.h"
#include "BreadGui.h"
#include <malloc.h>
#include <Imm.h>
#include <objbase.h>


namespace Bread
{
	#define CHT_IMEFILENAME1    _T("TINTLGNT.IME") // New Phonetic
	#define CHT_IMEFILENAME2    _T("CINTLGNT.IME") // New Chang Jie
	#define CHT_IMEFILENAME3    _T("MSTCIPHA.IME") // Phonetic 5.1
	#define CHS_IMEFILENAME1    _T("PINTLGNT.IME") // MSPY1.5/2/3
	#define CHS_IMEFILENAME2    _T("MSSCIPYA.IME") // MSPY3 for OfficeXP

	#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
	#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
	#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
	#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
	#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
	#define MAKEIMEVERSION( major, minor ) ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )
	#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
	#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
	#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )	// New ChanJie IME98b          : 4.4.x.x // WinXP
	#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
	#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
	#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
	#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
	#define IMEID_CHS_VER41	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
	#define IMEID_CHS_VER42	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )	// MSPY2	// Win2k/WinME
	#define IMEID_CHS_VER53	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )	// MSPY3	// WinXP

	enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
	enum { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

	#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)

	static wchar_t indicator[5][3] =  
	{
		L"En",
		L"\x7B80",
		L"\x7E41",
		L"\xAC00",
		L"\x3042",
	};

	/*
	*/
	cInputLocaleInfo::cInputLocaleInfo() : m_dllIme(NULL)
	{
	}
	/*
	*/
	cInputLocaleInfo::~cInputLocaleInfo()
	{
	}
	/*
	*/
	void cInputLocaleInfo::initialize(HWND hWnd)
	{
		onChangeLanguage(hWnd, 0, (LPARAM)GetKeyboardLayout(0));
		m_imm.Init();
	}
	/*
	*/
	int cInputLocaleInfo::renderDebugInfo(int y)
	{
		std_string outStr = m_inputStr + m_compStr + _T("|");
		_getFontMgr()->drawD3DText(10, y+=16, _T("[ime]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		_getFontMgr()->drawD3DText(10, y+=16, outStr.c_str(), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		_getFontMgr()->drawD3DText(10, y+=16, m_readingStr.c_str(), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		return y;
	}
	/*
	*/
	int cInputLocaleInfo::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_INPUTLANGCHANGE:	onChangeLanguage(hWnd, wParam, lParam);	break;
		// 응용 프로그램이 활성/비활성 될 때
		case WM_IME_SETCONTEXT:		break;
		// 조립 문자열을 만들기 직전/
		case WM_IME_STARTCOMPOSITION:	return 0;
		// 조립 상태가 변경 될 때
		case WM_IME_COMPOSITION:	onImeComposition(hWnd, wParam, lParam);	return 0;
		// 조립이 끝났을 때
		case WM_IME_ENDCOMPOSITION:	onImeEndComposition(hWnd, wParam, lParam);	return 0;
		// 응용 프로그램이 활성/비활성 될 때
		case WM_IME_NOTIFY:			onImeNotify(hWnd, wParam, lParam);	return 0;
		// 문자 하나가 완성되었을 때
		case WM_IME_CHAR:			break;
		case WM_CHAR:				onImeChar(hWnd, wParam, lParam);	break;
		}
		return 1;
	}
	/*
	*/
	void cInputLocaleInfo::onChangeLanguage(HWND hWnd, WPARAM /*wParam*/, LPARAM lParam)
	{
		m_hkl = (HKL)lParam;
		m_langId = LOWORD(m_hkl);
		m_codePage = (WORD)getCodePageFromLang(m_langId);
		m_inputStr.resize(0);
		DWORD prop = ImmGetProperty(GetKeyboardLayout(0), IGP_PROPERTY);
		m_unicodeIME = (prop & IME_PROP_UNICODE) ? true : false;
		checkToggleState(hWnd);
		getImeId();
		setupImeApi(hWnd);
	}
	/*
	*/
	void cInputLocaleInfo::getImeId()
	{
		TCHAR szTmp[1024];

		m_id[0] = m_id[1] = 0;

		if(!((m_hkl==_CHT_HKL) || (m_hkl==_CHT_HKL2) || (m_hkl==_CHS_HKL)))
			return;

		if ( !ImmGetIMEFileName( m_hkl, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
			return;

		if ( !_getReadingString ) 
		{
			if( ( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
				( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
				( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
				( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
				( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) ) 
			{
					return;
			}
		}

		DWORD   dwVerHandle;
		DWORD   dwVerSize = GetFileVersionInfoSize( szTmp, &dwVerHandle );

		if( dwVerSize ) {

			LPVOID  lpVerBuffer = alloca( dwVerSize );

			if( GetFileVersionInfo( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) ) {

				LPVOID  lpVerData;
				UINT    cbVerData;

				if( VerQueryValue( lpVerBuffer, _T("\\"), &lpVerData, &cbVerData ) ) {

					DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
					dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
					if( _getReadingString
						||
						( m_langId == LANG_CHT &&
						( dwVer == MAKEIMEVERSION(4, 2) || 
						dwVer == MAKEIMEVERSION(4, 3) || 
						dwVer == MAKEIMEVERSION(4, 4) || 
						dwVer == MAKEIMEVERSION(5, 0) ||
						dwVer == MAKEIMEVERSION(5, 1) ||
						dwVer == MAKEIMEVERSION(5, 2) ||
						dwVer == MAKEIMEVERSION(6, 0) ) )
						||
						( m_langId == LANG_CHS &&
						( dwVer == MAKEIMEVERSION(4, 1) ||
						dwVer == MAKEIMEVERSION(4, 2) ||
						dwVer == MAKEIMEVERSION(5, 3) ) ) ) {

							m_id[0] = dwVer | m_langId;
							m_id[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
					}
				}
			}
		}
	}
	/*
	*/
	void cInputLocaleInfo::setupImeApi(HWND hWnd)
	{
		char szImeFile[MAX_PATH + 1];

		_getReadingString = NULL;
		_showReadingWindow = NULL;

		if( ImmGetIMEFileNameA( m_hkl, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) != 0 ) {

			if( m_dllIme ) FreeLibrary( m_dllIme );

			m_dllIme = LoadLibraryA( szImeFile );

			if ( m_dllIme ) {
				_getReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
					( GetProcAddress( m_dllIme, "GetReadingString" ) );
				_showReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
					( GetProcAddress( m_dllIme, "ShowReadingWindow" ) );

				if( _showReadingWindow ) {
					HIMC hImc = ImmGetContext(hWnd);
					if(hImc) {
						_showReadingWindow( hImc, false );
						ImmReleaseContext(hWnd, hImc);
					}
				}
			}
		}
	}
	/*
	*/
	int cInputLocaleInfo::getCodePageFromLang(LANGID langId)
	{
		return getCodePageFromCharset(getCharsetFromLang(langId));
	}
	/*
	*/
	int	cInputLocaleInfo::getCodePageFromCharset(int charset)
	{
		switch( charset )
		{
		case SHIFTJIS_CHARSET:		return 932;
		case HANGUL_CHARSET:		return 949;
		case GB2312_CHARSET:		return 936;
		case CHINESEBIG5_CHARSET:	return 950;
		case GREEK_CHARSET:			return 1253;
		case TURKISH_CHARSET:		return 1254;
		case HEBREW_CHARSET:		return 1255;
		case ARABIC_CHARSET:		return 1256;
		case BALTIC_CHARSET:		return 1257;
		case THAI_CHARSET:			return 874;
		case EASTEUROPE_CHARSET:	return 1250;
		default:					return 1252;
		}
	}
	/*
	*/
	int	cInputLocaleInfo::getCharsetFromLang(LANGID langid)
	{
		switch( PRIMARYLANGID(langid) )
		{
		case LANG_JAPANESE:	return SHIFTJIS_CHARSET;
		case LANG_KOREAN:	return HANGEUL_CHARSET;
		case LANG_CHINESE:	switch( SUBLANGID(langid) )
							{
							case SUBLANG_CHINESE_SIMPLIFIED:
								return GB2312_CHARSET;
							case SUBLANG_CHINESE_TRADITIONAL:
								return CHINESEBIG5_CHARSET;
							default:
								return ANSI_CHARSET;
							}
		case LANG_GREEK:	return GREEK_CHARSET;
		case LANG_TURKISH:	return TURKISH_CHARSET;
		case LANG_HEBREW:	return HEBREW_CHARSET;
		case LANG_ARABIC:	return ARABIC_CHARSET;
		case LANG_ESTONIAN:
		case LANG_LATVIAN:
		case LANG_LITHUANIAN:	return BALTIC_CHARSET;
		case LANG_THAI:			return THAI_CHARSET;
		case LANG_CZECH:
		case LANG_HUNGARIAN:
		case LANG_POLISH:
		case LANG_CROATIAN:
		case LANG_MACEDONIAN:
		case LANG_ROMANIAN:
		case LANG_SLOVAK:
		case LANG_SLOVENIAN:	return EASTEUROPE_CHARSET;
		case LANG_RUSSIAN:
		case LANG_BELARUSIAN:
		case LANG_BULGARIAN:
		case LANG_UKRAINIAN:	return RUSSIAN_CHARSET;
		default:				return ANSI_CHARSET;
		}
	}
	/*
	*/
	void cInputLocaleInfo::checkToggleState(HWND hWnd)
	{
		/* Update Indicator */ 
		switch (PRIMARYLANGID(m_langId)) 
		{
		case LANG_KOREAN:
			m_verticalCandidate = false;
			m_curIndicator = indicator[INDICATOR_KOREAN];
			break;
		case LANG_JAPANESE:
			m_verticalCandidate = true;
			m_curIndicator = indicator[INDICATOR_JAPANESE];
			break;
		case LANG_CHINESE:
			m_verticalCandidate = true;
			switch(SUBLANGID(m_langId)) 
			{
			case SUBLANG_CHINESE_SIMPLIFIED:
				m_verticalCandidate = m_id[0] == 0;
				m_curIndicator = indicator[INDICATOR_CHS];
				break;
			case SUBLANG_CHINESE_TRADITIONAL:
				m_curIndicator = indicator[INDICATOR_CHT];
				break;
			default:
				m_curIndicator = indicator[INDICATOR_NON_IME];
				break;
			}
			break;
		default:
			m_curIndicator = indicator[INDICATOR_NON_IME];
			break;
		}

		if( m_curIndicator == indicator[INDICATOR_NON_IME] )
		{
			char szLang[10];
			GetLocaleInfoA( MAKELCID( m_langId, SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, szLang, sizeof(szLang) );
			m_curIndicator[0] = szLang[0];
			m_curIndicator[1] = towlower( szLang[1] );
		}

		/* Check Toggle State */ 
		bool bIme = ImmIsIME( m_hkl ) != 0;

		HIMC hImc = ImmGetContext(hWnd);
		if( hImc ) {

			if( ( PRIMARYLANGID(m_langId) == LANG_CHINESE ) && bIme ) {

				DWORD dwConvMode, dwSentMode;
				ImmGetConversionStatus(hImc, &dwConvMode, &dwSentMode);
				m_imeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;

			} else {

				m_imeState = ( bIme && ImmGetOpenStatus(hImc) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
			}
			ImmReleaseContext(hWnd, hImc);

		} else {

			m_imeState = IMEUI_STATE_OFF;
		}
	}
	/*
	*/
	void cInputLocaleInfo::onImeComposition(HWND hWnd, WPARAM /*wParam*/, LPARAM lParam)
	{
		HIMC hImc;

		TCHAR temp[4];
		if(lParam & GCS_COMPSTR) 
		{
			hImc = ImmGetContext(hWnd);
			if (hImc)
			{
				int tempSize = ImmGetCompositionString(hImc, GCS_COMPSTR, NULL, 0);
				if (tempSize > 0)
				{
					ImmGetCompositionString(hImc, GCS_COMPSTR, temp, tempSize);
					m_compStr.assign(temp, temp+tempSize/sizeof(TCHAR));

					ImmReleaseContext(hWnd, hImc);
				}
			}
		}
		if(lParam & GCS_RESULTSTR) 
		{
			hImc = ImmGetContext(hWnd);
			if (hImc)
			{
				int tempSize = ImmGetCompositionStringW(hImc, GCS_RESULTSTR, NULL, 0);
				if (tempSize > 0)
				{
					ImmGetCompositionString(hImc, GCS_RESULTSTR, temp, tempSize);
					m_inputStr.append(temp, temp+tempSize/sizeof(wchar_t));

					ImmReleaseContext(hWnd, hImc);
				}
			}
		}
		if(lParam & GCS_COMPATTR) 
		{
			hImc = ImmGetContext(hWnd);
			if (hImc)
			{
				int tempSize = ImmGetCompositionStringW(hImc, GCS_COMPATTR, NULL, 0);
				if (tempSize > 0)
				{
					ImmGetCompositionString(hImc, GCS_COMPATTR, temp, tempSize);

					int start, end;
					for(start=0; start<tempSize; ++start)	if(temp[start]==ATTR_TARGET_CONVERTED || temp[start]==ATTR_TARGET_NOTCONVERTED) break;
					for(end=start; end<tempSize; ++end)		if(temp[end]!=temp[start]) break;

					m_gcsStart	= start;
					m_gcsEnd	= end;

					ImmReleaseContext(hWnd, hImc);
				}
			}
		}
	}
	/*
	*/
	void cInputLocaleInfo::onImeEndComposition(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		m_compStr.resize(0);
		m_gcsStart = m_gcsEnd = 0;
		m_readingStr.resize(0);
	}
	/*
	*/
	void cInputLocaleInfo::onImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		HIMC hImc;

		switch (wParam) 
		{
		case IMN_OPENCANDIDATE:
		case IMN_CHANGECANDIDATE:
			hImc = ImmGetContext(hWnd);
			if(hImc) 
			{ 
				m_readingStr.resize(0);
				int candidateLen = ImmGetCandidateListW(hImc, 0, NULL, 0);
				if(candidateLen > 0) 
				{ 
					m_candidate.resize(candidateLen);
					ImmGetCandidateListW(hImc, 0, (CANDIDATELIST*)&m_candidate[0], candidateLen);
				}

				ImmReleaseContext(hWnd, hImc);
			}
			break;

		case IMN_CLOSECANDIDATE:
			m_candidate.resize(0);
			break;

		case IMN_SETCONVERSIONMODE:
		case IMN_SETOPENSTATUS:
			checkToggleState(hWnd);
			break;

		case IMN_PRIVATE:
			getPrivateReadingString(hWnd);

			// Trap some messages to hide reading window
			switch( m_id[0] )
			{
			case IMEID_CHT_VER42:
			case IMEID_CHT_VER43:
			case IMEID_CHT_VER44:
			case IMEID_CHS_VER41:
			case IMEID_CHS_VER42:
				if((lParam==1)||(lParam==2)) return ;
				break;

			case IMEID_CHT_VER50:
			case IMEID_CHT_VER51:
			case IMEID_CHT_VER52:
			case IMEID_CHT_VER60:
			case IMEID_CHS_VER53:
				if((lParam==16)||(lParam==17)||(lParam==26)||(lParam==27)||(lParam==28)) return ;
				break;
			}
			break;
		}
	}
	/*
	*/
	void cInputLocaleInfo::getPrivateReadingString(HWND hWnd)
	{
		if( !m_id[0] ) 
		{
			m_readingStr.resize(0);
			return;
		}

		HIMC hImc = ImmGetContext(hWnd);
		if( !hImc ) 
		{
			m_readingStr.resize(0);
			return;
		}

		DWORD dwErr = 0;
		if( _getReadingString ) 
		{
			UINT uMaxUiLen;
			BOOL bVertical;
			// Obtain the reading string size
			int wstrLen = _getReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );

			if( wstrLen == 0 ) 
			{
				m_readingStr.resize(0);
			} else 
			{
				wchar_t *wstr = (wchar_t*)alloca(sizeof(wchar_t) * wstrLen);
				_getReadingString( hImc, wstrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
				m_readingStr.assign(wstr, wstr+wstrLen);
			}

			m_verticalReading = bVertical ? true : false;

			ImmReleaseContext(hWnd, hImc);
		} 
		else 
		{

			// IMEs that doesn't implement Reading String API
			wchar_t* temp = NULL;
			DWORD tempLen = 0;
			bool bUnicodeIme = false;
			INPUTCONTEXT *lpIC = m_imm.LockIMC(hImc);

			if(lpIC == NULL) 
			{
				temp = NULL;
				tempLen = 0;
			} 
			else 
			{
				LPBYTE p = 0;
				switch( m_id[0] )
				{
				case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
				case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
				case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
					p = *(LPBYTE *)((LPBYTE)m_imm.LockIMCC( lpIC->hPrivate ) + 24 );
					if( !p ) break;
					tempLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
					dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
					temp = (wchar_t *)( p + 56 );
					bUnicodeIme = true;
					break;

				case IMEID_CHT_VER50: // 5.0.x.x // WinME
					p = *(LPBYTE *)( (LPBYTE)m_imm.LockIMCC( lpIC->hPrivate ) + 3 * 4 );
					if( !p ) break;
					p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
					if( !p ) break;
					tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
					temp = (wchar_t *)(p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = false;
					break;

				case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
				case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
				case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
					p = *(LPBYTE *)((LPBYTE)m_imm.LockIMCC( lpIC->hPrivate ) + 4);
					if( !p ) break;
					p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
					if( !p ) break;
					tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
					temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = true;
					break;

					// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
				case IMEID_CHS_VER41:
					{
						int nOffset;
						nOffset = ( m_id[1] >= 0x00000002 ) ? 8 : 7;

						p = *(LPBYTE *)((LPBYTE)m_imm.LockIMCC( lpIC->hPrivate ) + nOffset * 4);
						if( !p ) break;
						tempLen = *(DWORD *)(p + 7*4 + 16*2*4);
						dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
						dwErr = min( dwErr, tempLen );
						temp = (wchar_t *)(p + 6*4 + 16*2*1);
						bUnicodeIme = true;
					}
					break;

				case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
					{
						OSVERSIONINFOA osi;
						osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
						GetVersionExA( &osi );

						int nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(wchar_t) : sizeof(char);
						p = *(LPBYTE *)((LPBYTE)m_imm.LockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
						if( !p ) break;
						tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
						dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
						temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
						bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
					}
					break;

				default:
					temp = NULL;
					tempLen = 0;
					break;
				}
			}

			if(tempLen == 0) 
			{
				m_readingStr.resize(0);
			} 
			else 
			{
				if( bUnicodeIme ) 
				{
					m_readingStr.assign(temp, tempLen);
				} 
				else 
				{
					int wstrLen = MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, NULL, 0); 
					wchar_t* wstr = (wchar_t*)alloca(sizeof(wchar_t)*wstrLen);
					MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, wstr, wstrLen); 
					m_readingStr.assign(wstr, wstrLen);
				}
			}

			m_imm.UnlockIMCC(lpIC->hPrivate);
			m_imm.UnlockIMC(hImc);

			m_verticalReading = !getReadingWindowOrientation();
		}

		ImmReleaseContext(hWnd, hImc);
	}
	/*
	*/
	bool cInputLocaleInfo::getReadingWindowOrientation()
	{
		bool bHorizontalReading = ( m_hkl == _CHS_HKL ) || ( m_hkl == _CHT_HKL2 ) || ( m_id[0] == 0 );
		if( !bHorizontalReading && ( m_id[0] & 0x0000FFFF ) == LANG_CHT )
		{
			char szRegPath[MAX_PATH];
			HKEY hKey;
			DWORD dwVer = m_id[0] & 0xFFFF0000;
			strcpy( szRegPath, "software\\microsoft\\windows\\currentversion\\" );
			strcat( szRegPath, ( dwVer >= MAKEIMEVERSION( 5, 1 ) ) ? "MSTCIPH" : "TINTLGNT" );
			LONG lRc = RegOpenKeyExA( HKEY_CURRENT_USER, szRegPath, 0, KEY_READ, &hKey );
			if (lRc == ERROR_SUCCESS)
			{
				DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
				lRc = RegQueryValueExA( hKey, "Keyboard Mapping", NULL, &dwType, (PBYTE)&dwMapping, &dwSize );
				if (lRc == ERROR_SUCCESS)
				{
					if ( ( dwVer <= MAKEIMEVERSION( 5, 0 ) && 
						( (BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23 ) )
						||
						( ( dwVer == MAKEIMEVERSION( 5, 1 ) || dwVer == MAKEIMEVERSION( 5, 2 ) ) &&
						(BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24 )
						)
					{
						bHorizontalReading = true;
					}
				}
				RegCloseKey( hKey );
			}
		}

		return bHorizontalReading;
	}
	/*
	*/
	void cInputLocaleInfo::onImeChar(HWND /*hWnd*/, WPARAM wParam, LPARAM /*lParam*/)
	{
		switch(wParam) 
		{
		case '\r':
		case '\n':
			m_inputStr.resize(0);
			break;
		case '\b':
			if(m_inputStr.size()) m_inputStr.resize(m_inputStr.size() - 1);
			break;
		case '\t':
		case 27:
			break;
		default:
			if(wParam > 31) 
			{
				wchar_t temp;
				MultiByteToWideChar(m_codePage, 0, (char*)&wParam, 1, &temp, 1);
				m_inputStr.push_back(temp);
			}
			break;
		}
	}
	/*
	*/
	int cInputLocaleInfo::getCandidate(DWORD index, TCHAR* text, int len) 
	{
		if(m_candidate.empty()) 
		{
			return 0;
		} 
		else 
		{
			CANDIDATELIST* candidateList = (CANDIDATELIST*)&m_candidate[0];

			if(index >= candidateList->dwCount) 
			{
				return 0;
			} 
			else 
			{
				if(m_unicodeIME) 
				{
					TCHAR* wText = (TCHAR*)(&m_candidate[0] + candidateList->dwOffset[index]);
					_tcsncpy(text, wText, len);
					return _tcslen(text);
				} 
				else 
				{
					char* temp = (char*)(&m_candidate[0] + candidateList->dwOffset[index]);

					if(text == 0) 
					{
						return strlen(temp);
					} 
					else 
					{
						int tempLen = strlen(temp);

						if(len < tempLen) 
						{
							return 0;
						} 
						else 
						{
							memcpy(text, temp, tempLen);
							return tempLen;
						}
					}
				}
			}
		}
	}
}