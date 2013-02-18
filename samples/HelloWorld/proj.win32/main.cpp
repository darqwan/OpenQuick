#include "QDefines.h" // include this first, it turns off various VS warnings
#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

USING_NS_CC;

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

void setupConsole()
{
	int consoleHandle;
	long osFileHandle;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	FILE *fp;

	//search for parent console
	BOOL consoleReady = AttachConsole(ATTACH_PARENT_PROCESS);
	if(!consoleReady)
	{
		//how about allocating a new one
		consoleReady = AllocConsole();
	}
	if(consoleReady)
	{
		//update buffer size
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
		consoleInfo.dwSize.Y = 1200;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleInfo.dwSize);

		//redirect unbuffered STDOUT to the console
		osFileHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
		consoleHandle = _open_osfhandle(osFileHandle, _O_TEXT);
		fp = _fdopen(consoleHandle, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);

		//redirect unbuffered STDIN to the console
		osFileHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
		consoleHandle = _open_osfhandle(osFileHandle, _O_TEXT);
		fp = _fdopen(consoleHandle, "r");
		*stdin = *fp;
		setvbuf(stdin, NULL, _IONBF, 0);

		//redirect unbuffered STDERR to the console
		osFileHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
		consoleHandle = _open_osfhandle(osFileHandle, _O_TEXT);
		fp = _fdopen(consoleHandle, "w");
		*stderr = *fp;
		setvbuf(stderr, NULL, _IONBF, 0);

		//make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
		ios::sync_with_stdio();
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	setupConsole();
#endif

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize(WINDOW_ACTUAL_W, WINDOW_ACTUAL_H);

    return CCApplication::sharedApplication()->run();
}
