#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileReader.h"
#include "Sample.h"
#include "Instrument.h"
#include "Session.h"
#include "JACKSoundSystem.h"

#define CLIENT_NAME "CMI"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    _ss = new CMI::ASIOSoundSystem();
//    _ss = new CMI::WASAPISoundSystem(30);
    _ss = new CMI::JACKSoundSystem(CLIENT_NAME);
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

//    _midiController = new CMI::WinMIDIController();
//    _midiController->Initialise(0, 0);
//    _midiController->SetSoundOutput(_ss);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

//    _midiController->Finalise();
//    delete _midiController;
    _ss->Finalise();
    delete _ss;
}
