
#ifndef _BreadInputLocaleInfo_h_
#define _BreadInputLocaleInfo_h_

#include <Windows.h>
#include "BreadDeclaration.h"
#include "BreadImm.h"

namespace Bread
{
	/*
		@date 2010.07.05
		@auth prodongi
		@desc ime 국가별 설정
		@todo zupet님의 오픈 소스에서 참조
	*/
	class cInputLocaleInfo
	{
	public:
		cInputLocaleInfo();
		~cInputLocaleInfo();
		void initialize(HWND hWnd);
		int renderDebugInfo(int y);
		std_string const& getInputStr() const	{	return m_inputStr;	}
		std_string const& getCompStr() const	{	return m_compStr;	}
		std_string const& getReadingStr() const	{	return m_readingStr;}
		WORD getCodePage() const				{	return m_codePage;	}
		int wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void onChangeLanguage(HWND hWnd, WPARAM wParam, LPARAM lParam);
		int getCodePageFromLang(LANGID langId);
		int getCodePageFromCharset(int charset);
		int getCharsetFromLang(LANGID langid);
		void getImeId();
		void setupImeApi(HWND hWnd);
		void checkToggleState(HWND hWnd);
		void onImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
		void onImeEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
		void onImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
		void onImeChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
		void getPrivateReadingString(HWND hWnd);
		bool getReadingWindowOrientation();
		int getCandidate(DWORD index, TCHAR* text, int len);

	private:
		enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
		enum { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

		HKL m_hkl;
		LANGID m_langId;
		WORD m_codePage;
		bool m_unicodeIME;
		bool m_verticalCandidate;
		wchar_t* m_curIndicator;
		DWORD m_id[2];
		int m_imeState;
		HINSTANCE m_dllIme;

		// ime
		std_string m_compStr;
		std_string m_readingStr;
		std_string m_inputStr;
		std::vector<BYTE> m_candidate;
		int m_gcsStart;
		int m_gcsEnd;
		bool m_verticalReading;
		cIMM m_imm;

		UINT (WINAPI * _getReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
		BOOL (WINAPI * _showReadingWindow)( HIMC, BOOL );
	};
}




#endif