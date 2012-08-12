#-------------------------------------------------
#
# Project created by QtCreator 2012-08-12T11:46:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AsioTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    WaveAudioLoader.cpp \
    WaveAudioEssenceReader.cpp \
    UL.cpp \
    Timecode.cpp \
    SoundSystem.cpp \
    Session.cpp \
    SampleStream.cpp \
    Sample.cpp \
    RIFFReader.cpp \
    RIFFChunk.cpp \
    Rational.cpp \
    Instrument.cpp \
    FileReader.cpp \
    MIDIController.cpp \
    EssenceReader.cpp \
    conversion.cpp \
    JACKSoundSystem.cpp

HEADERS  += mainwindow.h \
    WaveAudioLoader.h \
    WaveAudioEssenceReader.h \
    UL.h \
    UID.h \
    Timecode.h \
    SoundSystem.h \
    Session.h \
    SampleStream.h \
    Sample.h \
    RIFFReader.h \
    RIFFChunk.h \
    resource.h \
    Reader.h \
    Rational.h \
    MIDIController.h \
    IOBase.h \
    Instrument.h \
    FileReader.h \
    Exception.h \
    EssenceReader.h \
    EssenceErrorHandler.h \
    EssenceContentHandler.h \
    errorcodes.h \
    EditUnitSize.h \
    conversion.h \
    common.h \
    JACKSoundSystem.h

FORMS    += mainwindow.ui

LIBS += -lsamplerate
