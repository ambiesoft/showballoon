#include "stdafx.h"
#include "../MyUtility/UTF16toUTF8.h"
#include "../MyUtility/showballoon.h"



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
	// /title:"My Title" /duration:10000 "Hello World!"
	if(__argc < 2)
	{
		wstring message;
		message += NS(L"Usage:\r\n");
		message += NS(L"showballoon.exe [/title:STRING] [/icon:EXE or DLL for ICON] [/iconindex:i] [/duration:MILLISEC] [/waitpid:PID] STRING");
		message += L"\r\n";
		message += L"\r\n";
		message += L"STRING:\tUTF8 url encoded string";
		message += L"\r\n";
		message += L"RESOURCEFORICON:\tExe or Dll for finding icon";
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
	int duration=5000;
	int waitpid=0;
	UINT uTrayID=WM_APP+1;

	LPCSTR pTitleOption = "/title:";
	size_t nTitleOption = strlen(pTitleOption);
	
	LPCSTR pIconOption = "/icon:";
	size_t nIconOption = strlen(pIconOption);

	LPCSTR pIconIndexOption = "/iconindex:";
	size_t nIconIndexOption = strlen(pIconIndexOption);

	LPCSTR pDurationOption = "/duration:";
	size_t nDurationOption = strlen(pDurationOption);

	LPCSTR pWaitpidOption = "/waitpid:";
	size_t nWaitpidOption = strlen(pWaitpidOption);
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
	}

	showballoon(NULL,title,text,sfi.hIcon, duration, uTrayID);
	DestroyIcon(sfi.hIcon);
	return 0;
}
