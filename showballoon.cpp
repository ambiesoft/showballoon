// showballoon.cpp :
//

#include "stdafx.h"


#include "../MyUtility/UTF16toUTF8.h"

#pragma comment(lib,"Comctl32.lib")

using namespace std;

HICON ghIcon;

BOOL NotifyIconize(UINT uID, DWORD dwMessage, LPCWSTR pInfoTitle , LPCWSTR pInfo)
{
	NOTIFYICONDATAW nid;
	ZeroMemory(&nid,sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATAW);
	nid.hWnd = NULL;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | 0x00000010;
	nid.dwInfoFlags      = 0x00000001;
	nid.uTimeout         = 300;
	nid.uCallbackMessage = 0; //WM_APP_TRAYMESSAGE;
	nid.hIcon = ghIcon; //LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
	lstrcpyW(nid.szTip, L"dater");
	if(pInfoTitle)
		lstrcpyW( nid.szInfoTitle, pInfoTitle );
	if(pInfo)
		lstrcpyW( nid.szInfo, pInfo );
	
	BOOL ret= Shell_NotifyIconW( dwMessage,&nid);
	//DestroyIcon(sfi.hIcon);
	return ret;
}

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
	
	if(__argc < 2)
	{
		wstring message;
		message += NS(L"Usage:\r\n");
		message += NS(L"showballoon.exe [/title:STRING] [/icon:RESOURCEFORICON] [/iconindex:i] STRING");
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
	UINT uTrayID=WM_APP+1;

	LPCSTR pTitleOption = "/title:";
	size_t nTitleOption = strlen(pTitleOption);
	
	LPCSTR pIconOption = "/icon:";
	size_t nIconOption = strlen(pIconOption);

	LPCSTR pIconIndexOption = "/iconindex:";
	size_t nIconIndexOption = strlen(pIconIndexOption);
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
		else
		{
			text = argToWstring(__argv[i]);
		}
	}

	CoInitialize(NULL);

	if(!iconexe.empty())
	{
		SHFILEINFOW sfi={0};
		SHGetFileInfoW(iconexe.c_str(),
					   0,
					   &sfi, 
					   sizeof(SHFILEINFO), 
					   // SHGFI_SYSICONINDEX|
					   SHGFI_ICON|
					   SHGFI_SMALLICON);


		ghIcon = sfi.hIcon;
	} 	
	

	InitCommonControls();
	HWND hBalloon = CreateWindowW(L"tooltips_class32",
		NULL,
		WS_POPUP|0x02|0x40|0x01,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL);
	if(!hBalloon)
	{
		MessageBoxW(NULL, L"balloon",NULL,MB_ICONERROR);
		return __LINE__;
	}
	if(!SendMessageW(hBalloon,
		TTM_SETTITLE, // Adds a standard icon and title string to a ToolTip    
		1,
		(LPARAM)L"title"))
	{
		MessageBoxW(NULL, L"balloon sendmessage",NULL,MB_ICONERROR);
		return __LINE__;
	}

	NotifyIconize(uTrayID,NIM_DELETE, NULL, NULL);
	if(!NotifyIconize(uTrayID,NIM_ADD, NULL, NULL))
	{
		MessageBoxA(NULL, "NotifyAdd",NULL,MB_ICONERROR);
		return __LINE__;
	}



	if(!NotifyIconize(uTrayID, NIM_MODIFY, title.c_str(), text.c_str() ))
	{
		MessageBoxA(NULL, "NotifyModify",NULL,MB_ICONERROR);
		return __LINE__;
	}
	Sleep(5000);
	NotifyIconize(uTrayID, NIM_DELETE, NULL, NULL);
	DestroyWindow(hBalloon);
	DestroyIcon(ghIcon);
	return 0;
}



