//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.


#include "stdafx.h"

#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <stlsoft/smartptr/scoped_handle.hpp>

#include "../lsMisc/UTF16toUTF8.h"
#include "../lsMisc/showballoon.h"
#include "../lsMisc/CommandLineString.h"
#include "../lsMisc/stdwin32/stdwin32.h"

using namespace stdwin32;
using namespace Ambiesoft;


char a2c1(char c1)
{
	char ret=0;
	if('0'<=c1 && c1<='9')
	{
		ret=c1-'0';
	}
	else 
	{
		switch(c1)
		{
		case 'a': case 'A': ret=0xa; break;
		case 'b': case 'B': ret=0xb; break;
		case 'c': case 'C': ret=0xc; break;
		case 'd': case 'D': ret=0xd; break;
		case 'e': case 'E': ret=0xe; break;
		case 'f': case 'F': ret=0xf; break;
		}
	}
	return ret;
}

char a2c(char c1, char c2)
{
	return a2c1(c1)*16 + a2c1(c2);
}
	

wstring argToWstring(const char* p)
{
	string org;
	for(;*p;++p)
	{
		if(*p=='%')
		{
			if( *(p+1)==0 || *(p+2)==0)
				break;

			char c=a2c(*(p+1), *(p+2));
			org += c;
			p+=2;
		}
		else if(*p=='+')
		{
			org+=' ';
		}
		else
		{
			org+=*p;
		}
	}
	wstring ret;
	UTF8toUTF16((const LPBYTE)org.c_str(),ret);
	return ret;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
	// /title:"My Title" /duration:10000 "Hello World!" /balloonicon:3
	if(__argc < 2)
	{
		wstring message;
		message += NS(L"Usage:\r\n");
		message += NS(L"showballoon.exe [/title:STRING] [/icon:[EXE|DLL|ICON]] [/iconindex:i | /defaulticon] [/duration:MILLISEC] [/waitpid:PID] [/balloonicon:BII] STRING");
		message += L"\r\n";
		message += L"\r\n";
		message += L"STRING:";
		message += L"\r\n";
		message += L"  UTF8 url encoded string";
		message += L"\r\n";
		message += L"\r\n";
		message += L"RESOURCEFORICON:";
		message += L"\r\n";
		message += L"  Exe or Dll for finding icon";
		message += L"\r\n";
		message += L"\r\n";
		message += L"BII:";
		message += L"\r\n";
		message += L"  0=None, 1=Info, 2=Warning, 3=Error";
		message += L"\r\n";
		message += L"\r\n";
		message += L"Visit https://github.com/ambiesoft/showballoon for more information.";
		MessageBoxW(NULL,
			message.c_str(),
			L"showballoon 1.04",
			MB_ICONINFORMATION);
		return 1;
	}

	wstring text;
	wstring title;
	wstring iconexe;
	int iconindex=0;
	bool defaulticon = false;
	int duration=5000;
	int waitpid=0;
	UINT uTrayID=GetTickCount();
	DWORD dwBalloonIcon=NIIF_NONE;

	LPCSTR pTitleOption = "/title:";
	size_t nTitleOption = strlen(pTitleOption);
	
	LPCSTR pIconOption = "/icon:";
	size_t nIconOption = strlen(pIconOption);

	LPCSTR pIconIndexOption = "/iconindex:";
	size_t nIconIndexOption = strlen(pIconIndexOption);

	LPCSTR pDefaultIconOption = "/defaulticon:";
	size_t nDefaultIconOption = strlen(pIconIndexOption);

	
	LPCSTR pDurationOption = "/duration:";
	size_t nDurationOption = strlen(pDurationOption);

	LPCSTR pWaitpidOption = "/waitpid:";
	size_t nWaitpidOption = strlen(pWaitpidOption);

	LPCSTR pBalloonIconOption = "/balloonicon:";
	size_t nBalloonIconOption = strlen(pBalloonIconOption);


	int argc = 0;
	LPSTR* argv = CCommandLineStringBase<char>::getCommandLineArg(GetCommandLineA(), &argc);
	stlsoft::scoped_handle<LPSTR*> myfreev(argv, CCommandLineStringBase<char>::freeCommandLineArg);
	for(int i=1 ; i < argc ; ++i)
	{
		if(0== _strnicmp(argv[i], pTitleOption, nTitleOption))
		{
			char* pS = argv[i] + nTitleOption;
			title=argToWstring(pS);
		}
		else if(0== _strnicmp(argv[i], pIconOption, nIconOption))
		{
			char* pS = argv[i] + nIconOption;
			iconexe=argToWstring(pS);
		}
		else if(0== _strnicmp(argv[i], pIconIndexOption, nIconIndexOption))
		{
			char* pS = argv[i] + nIconIndexOption;
			iconindex = atoi(pS);
		}
		else if(0== _strnicmp(argv[i], pDefaultIconOption, nDefaultIconOption))
		{
			defaulticon = true;
		}
		else if(0== _strnicmp(argv[i], pDurationOption, nDurationOption))
		{
			char* pS = argv[i] + nDurationOption;
			duration = atoi(pS);
		}
		else if(0== _strnicmp(argv[i], pWaitpidOption, nWaitpidOption))
		{
			char* pS = argv[i] + nWaitpidOption;
			waitpid = atoi(pS);
		}
		else if(0== _strnicmp(argv[i], pBalloonIconOption, nBalloonIconOption))
		{
			char* pS = argv[i] + nBalloonIconOption;
			dwBalloonIcon = atoi(pS);
		}


		else
		{
			text = argToWstring(argv[i]);
		}
	}

	if(waitpid!=0)
	{
		HANDLE ph = OpenProcess(SYNCHRONIZE, FALSE, waitpid);
		if(ph)
			WaitForSingleObject(ph,INFINITE);
	}


	HICON hIcon = NULL;
	if(!iconexe.empty())
	{
		
//		SHFILEINFOW sfi={0};
//		SHGetFileInfoW(iconexe.c_str(),
//					   0,
//					   &sfi, 
//					   sizeof(SHFILEINFO), 
//					   // SHGFI_SYSICONINDEX|
//					   SHGFI_ICON|
//					   SHGFI_SMALLICON);
//		hIcon = sfi.hIcon;


//		ExtractIconExW(
//			iconexe.c_str(),
//			iconindex,
//			NULL,
//			&hIcon,
//			1);

		hIcon = ExtractIconW(hInstance,
			iconexe.c_str(),
			iconindex);
	}
	else if(defaulticon)
	{
		SHFILEINFOW sfi={0};
		wstring thisexe = stdGetModuleFileNameW();
		SHGetFileInfoW(thisexe.c_str(),
					   0,
					   &sfi, 
					   sizeof(SHFILEINFO), 
					   // SHGFI_SYSICONINDEX|
					   SHGFI_ICON|
					   SHGFI_SMALLICON);

		hIcon = sfi.hIcon;
	}

	showballoon(
		NULL,
		title,
		text,
		hIcon,
		duration, 
		uTrayID,
		FALSE,
		dwBalloonIcon);
	
	if(hIcon)
		DestroyIcon(hIcon);
	return 0;
}
