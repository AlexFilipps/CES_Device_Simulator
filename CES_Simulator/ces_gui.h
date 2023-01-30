#ifndef CES_GUI_H
#define CES_GUI_H

#include <QMainWindow>
#include "def.h"
#include "controller.h"
#include <QVector>
#include <QLabel>
#include <QHBoxLayout>

class treatments; //forward declaration

QT_BEGIN_NAMESPACE
namespace Ui { class CES_GUI; }
QT_END_NAMESPACE

class CES_GUI : public QMainWindow
{
    Q_OBJECT

public:
    CES_GUI(QWidget *parent = nullptr);
    ~CES_GUI();
    void setBatteryLevel(float); //from 0-100
    void setRemainingTime(int); //time in seconds
    void enableElectrodesSymbol(bool); //true = enable, false = disable.
    void setPowerLevel(int); //int from 0-9
    void enableStartTreatmentButton(bool); //true = enable, false = disable.
    void toggleSaveTreatmentButton(bool); //true = "save treatment", false = "do not save treatment".
    void setTimeSetting(int); //int represents id, 0 = 20 mins, 1=40mins, 2=60mins
    void setWaveform(int);//int represents id, 0=alpha, 1=beta, 2=gamma
    void setFrequency(int); //int represents id, 0=0.5hz, 1=77hz, 2=100hz
    void updateTreatmentHistory(QVector<treatments*>*);//pointer to vector with treatments
    void toggleConnectEarlobeButtonText(bool); //true=>sets button's text to "connect...", false=> set's button to "disconnect..."
    void toggleTimeScaleButtonText(bool); //true=>sets button's text to "slow down time", false=> set's button to "speed up time"
    void enableTimeButton(bool); //true=>sets time button enabled, false=>sets time button disabled.
    void enableWaveformButton(bool); //true=>sets waveform button enabled, false=>sets waveform button disabled.
    void enableFreqButton(bool); //true=>sets freq button enabled, false=>sets freq button disabled.
    void enableOffScreen(bool); //true=>turns ON the off screen, false=>turns OFF the off screen (normal screen is on).
    void enableOnButton(bool); //true=>sets on button enabled, false=>sets on button disabled.

private:
    void addTreatmentToHistory(treatments*); //helper for updateTreatmentHistory
    void hideLowBatteryWarning(); //helper for setBatteryLevel
    void hideDeadBatteryWarning(); //helpter for setBatteryLevel
    Ui::CES_GUI *ui;
    Controller controller;
    QVBoxLayout* vertScrollArea;
    QVector<QLabel*> powerLevelBarNotches;
    int numHistoryEntriesBeingDisplayed;
    bool haveDisplayedLowBatteryWarning;
    bool haveDisplayedDeadBatteryWarning;
};
#endif // CES_GUI_H
