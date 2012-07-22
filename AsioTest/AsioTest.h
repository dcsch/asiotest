
// AsioTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

namespace CMI
{
	class SoundSystem;
	class MIDIController;
}

using namespace CMI;

// CAsioTestApp:
// See AsioTest.cpp for the implementation of this class
//

class CAsioTestApp :
	public CWinAppEx
{
public:
	CAsioTestApp();

	CMI::SoundSystem *GetSoundSystem();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()


private:
	CMI::SoundSystem *_ss;
	CMI::MIDIController *_midiController;
};

extern CAsioTestApp theApp;

inline CMI::SoundSystem *CAsioTestApp::GetSoundSystem()
{
	return _ss;
}
