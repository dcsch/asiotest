
// AsioTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "common.h"
#include "AsioTest.h"
#include "AsioTestDlg.h"
#include "ASIOSoundSystem.h"
#include "WASAPISoundSystem.h"
#include "FileReader.h"
#include "WinMIDIController.h"
#include "Session.h"
#include "Instrument.h"
#include "Sample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAsioTestApp

BEGIN_MESSAGE_MAP(CAsioTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAsioTestApp construction

CAsioTestApp::CAsioTestApp() :
	_ss(0)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAsioTestApp object

CAsioTestApp theApp;


// CAsioTestApp initialization

BOOL CAsioTestApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CWinAppEx::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

//	_ss = new CMI::ASIOSoundSystem();
	_ss = new CMI::WASAPISoundSystem(30);
	_ss->Initialise();

	CMI::FileReader fileReader;
	fileReader.open(L"C:\\CMI30A\\OPMWind\\sax-alto.wav");

	CMI::Sample *sample = new CMI::Sample();
	sample->Load(fileReader);
	fileReader.close();

	CMI::Instrument *instrument = new CMI::Instrument();
	instrument->setSample(0, sample);

	CMI::Session *session = new CMI::Session();
	session->setInstrument(0, instrument);

	_ss->setSession(session);

	_midiController = new CMI::WinMIDIController();
	_midiController->Initialise(0, 0);
	_midiController->SetSoundOutput(_ss);

	CAsioTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	_midiController->Finalise();
	delete _midiController;
	_ss->Finalise();
	delete _ss;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
