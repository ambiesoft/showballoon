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


#include <Windows.h>
#include <tchar.h>
#include <shlwapi.h>

#include <string.h>
#include <string>

#include "../../lsMisc/OpenCommon.h"
#include "../../lsMisc/stdosd/stdosd.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"shlwapi.lib")

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;


wstring strSB;
void testit(LPCTSTR pCommand)
{
	MessageBox(NULL,pCommand,_T("TEST"),MB_OK);
	OpenCommon(NULL, strSB.c_str(), pCommand);
}

int main()
{
	strSB = stdCombinePath(
		stdGetParentDirectory(stdGetModuleFileName()),
		L"showballoon.exe");
	if (!PathFileExists(strSB.c_str()))
	{
		MessageBox(NULL, L"NAI", 0, 0);
		return 1;
	}

	wstring command;
	
	command = L"ABC";
	testit(command.c_str());

	
	command = _T("%E3%81%88%E3%81%88%E3%81%84%E3%81%82%E3%81%82+%E5%90%9B%E3%81%8B%E3%82%89%E3%80%8C%E3%82%82%E3%82%89%E3%81%84%E6%B3%A3%E3%81%8D%E3%80%8D+%E3%81%BB%E3%82%8D%E3%82%8A%E3%83%BB%E3%81%BB%E3%82%8D%E3%82%8A+%E3%81%B5%E3%81%9F%E3%82%8A%E3%81%BC%E3%81%A3%E3%81%A1");
	testit(command.c_str());
	

	command = stdFormat(_T("/title:%s /icon:%s /defaulticon /balloonicon:%d /duration:%d %s"),
		_T("%E3%82%82%E3%82%89%E3%81%84%E6%B3%A3%E3%81%8D"), //title
		(wstring(_T("\"") + strSB + _T("\""))).c_str(), // icon
		1, // balloonicon
		60 * 1000,
		_T("%E3%81%88%E3%81%88%E3%81%84%E3%81%82%E3%81%82+%E5%90%9B%E3%81%8B%E3%82%89%E3%80%8C%E3%82%82%E3%82%89%E3%81%84%E6%B3%A3%E3%81%8D%E3%80%8D+%E3%81%BB%E3%82%8D%E3%82%8A%E3%83%BB%E3%81%BB%E3%82%8D%E3%82%8A+%E3%81%B5%E3%81%9F%E3%82%8A%E3%81%BC%E3%81%A3%E3%81%A1")
	);
	testit(command.c_str());


	command = stdFormat(_T("/title:%s /icon:%s /defaulticon /balloonicon:%d /duration:%d %s"),
		_T("%EC%98%81%EC%96%B4"), //title
		(wstring(_T("\"") + strSB + _T("\""))).c_str(), // icon
		1, // balloonicon
		60 * 1000,
		_T("%E8%8B%B1%E8%AF%AD%EF%BC%88English%EF%BC%8C%E8%81%86%E8%81%BDi%2F%CB%88%C9%AA%C5%8B%C9%A1l%C9%AA%CA%83%2F%EF%BC%89%E6%98%AF%E4%B8%80%E7%A7%8D%E8%A5%BF%E6%97%A5%E8%80%B3%E6%9B%BC%E8%AF%AD%E8%A8%80")
	);
	testit(command.c_str());
	return 0;
}