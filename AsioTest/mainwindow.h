#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

namespace CMI
{
    class SoundSystem;
    class MIDIController;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CMI::SoundSystem *getSoundSystem();

private:
    Ui::MainWindow *ui;
    CMI::SoundSystem *_ss;
    CMI::MIDIController *_midiController;
};

inline CMI::SoundSystem *MainWindow::getSoundSystem()
{
    return _ss;
}

#endif // MAINWINDOW_H
