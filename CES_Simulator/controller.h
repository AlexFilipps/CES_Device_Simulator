#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include "CES_Model.h"
#include "treatments.h"

class CES_GUI; //forward declaration

class Controller : public QObject{
    Q_OBJECT

public:
    Controller();
    void setGUI(CES_GUI* ui);
    void startController(); //Called when device is switched on
    void endController();   //Called when device is switched off

public slots:
    void powerButtonClicked();  //Switches the device on/off
    void update();              //The loop to be executed every second
    void startTreatmentPressed();       //Begins the treatment
    void increasePowerLevelPressed();   //Increases the power selection
    void decreasePowerLevelPressed();   //Dercreases the power selection
    void saveTreatmentPressed();        //Toggles treatment saving
    void cycleTimeOptionsButtonPressed();       //Cycles between duration options
    void cycleWaveformOptionsButtonPressed();   //Cycles between waveform options
    void cycleFreqOptionsButtonPressed();       //Cycles between frequency options
    void connectEarlobeClipsPressed();  //Toggles earlobe clip status
    void slowDownTimePressed();         //Toggles simulation sleep
    void shutDown();                    //Shutsdown device
    void powerSurgePressed();           //Simulates receiving a power surge signal

private:
    bool deviceOn;  //Whether the device is on/off
    bool sentShutDownSignal; //usually false, flips to true once battery < 2, we called started a timer to call shutdown.
    CES_GUI* gui;
    CES_Model* model;
    QTimer* timer;  //Timer used to update the device at 1 sec intervals
    int duration;   //Saves the duration of the treatment as a 0-2 index
    int waveform;   //Saves the waveform of the treatment as a 0-2 index
    int frequency;  //Saves the frequency of the treatment as a 0-2 index
    int power;      //Saves the power of the treatment as a 0-9 index
    bool saveTreatment;         //Whether the treatment should be saved upon termination or not
    bool electrodesConnected;   //Whether the electrodes are connected
    bool timeSpedUp;            //Whether the speed of the simulation is normal or sped up
};

#endif // CONTROLLER_H
