#include "stdafx.h"
#include "../MyUtility/UTF16toUTF8.h"
#include "../MyUtility/showballoon.h"
#include "../MyUtility/stdwin32/stdwin32.h"
using namespace stdwin32;



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

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
	// /title:"My Title" /duration:10000 "Hello World!" /balloonicon:3
	if(__argc < 2)
	{
		wstring message;
		message += NS(L"Usage:\r\n");
		message += NS(L"showballoon.exe [/title:STRING] [[/icon:[EXE|DLL|ICON]] [/iconindex:i]|[/defaulticon]] [/duration:MILLISEC] [/waitpid:PID] [/balloonicon:BII] STRING");
		message += L"\r\n";
		message += L"\r\n";
		message += L"STRING:\tUTF8 url encoded string";
		message += L"\r\n";
		message += L"RESOURCEFORICON:\tExe or Dll for finding icon";
		message += L"\r\n";
		message += L"BII:0=None, 1=Info, 2=Warning, 3=Error, (Default=1)";
		MessageBoxW(NULL, 
			message.c_str(),
			L"showballoon",
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
	UINT uTrayID=WM_APP+1;
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

	for(int i=1 ; i < __argc ; ++i)
	{
		if(0== strnicmp(__argv[i], pTitleOption, nTitleOption))
		{
			char* pS = __argv[i] + nTitleOption;
			title=argToWstring(pS);
		}
		else if(0== strnicmp(__argv[i], pIconOption, nIconOption))
		{
			char* pS = __argv[i] + nIconOption;
			iconexe=argToWstring(pS);
		}
		else if(0== strnicmp(__argv[i], pIconIndexOption, nIconIndexOption))
		{
			char* pS = __argv[i] + nIconIndexOption;
			iconindex = atoi(pS);
		}
		else if(0== strnicmp(__argv[i], pDefaultIconOption, nDefaultIconOption))
		{
			defaulticon = true;
		}
		else if(0== strnicmp(__argv[i], pDurationOption, nDurationOption))
		{
			char* pS = __argv[i] + nDurationOption;
			duration = atoi(pS);
		}
		else if(0== strnicmp(__argv[i], pWaitpidOption, nWaitpidOption))
		{
			char* pS = __argv[i] + nWaitpidOption;
			waitpid = atoi(pS);
		}
		else if(0== strnicmp(__argv[i], pBalloonIconOption, nBalloonIconOption))
		{
			char* pS = __argv[i] + nBalloonIconOption;
			dwBalloonIcon = atoi(pS);
		}


		else
		{
			text = argToWstring(__argv[i]);
		}
	}

	if(waitpid!=0)
	{
		HANDLE ph = OpenProcess(SYNCHRONIZE, FALSE, waitpid);
		if(ph)
			WaitForSingleObject(ph,INFINITE);
	}

	HICON hIcon = NULL;
	SHFILEINFOW sfi={0};
	if(!iconexe.empty())
	{
		SHGetFileInfoW(iconexe.c_str(),
					   0,
					   &sfi, 
					   sizeof(SHFILEINFO), 
					   // SHGFI_SYSICONINDEX|
					   SHGFI_ICON|
					   SHGFI_SMALLICON);
		hIcon = sfi.hIcon;
	}
	else if(defaulticon)
	{
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
		sfi.hIcon,
		duration, 
		uTrayID,
		FALSE,
		dwBalloonIcon);
	
	if(hIcon)
		DestroyIcon(hIcon);
	return 0;
}
