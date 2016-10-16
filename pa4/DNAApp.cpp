//
//  DNAApp.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNAApp.h"
#include "DNAFrame.h"
#include <iostream>

wxIMPLEMENT_APP(DNAApp);

#ifdef _WIN32
void RedirectIOToConsole()
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
		&coninfo);
	coninfo.dwSize.Y = 1000;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
		coninfo.dwSize);

	// Redirect stdio
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
}
#endif

bool DNAApp::OnInit()
{
	RedirectIOToConsole();
	mFrame = new DNAFrame( "DNAwx", wxPoint(50, 50), wxSize(1024, 768) );
	
	return true;
}
