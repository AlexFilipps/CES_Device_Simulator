#include "controller.h"
#include "ces_gui.h"
#include <QApplication>
#include <QDebug>
#include <QTimer>

Controller::Controller(){
    qInfo("Controller created!");
    deviceOn = false;
    duration = 0;
    waveform = 0;
    frequency = 0;
    power = 0;
    saveTreatment = false;
    electrodesConnected = false;
    timeSpedUp = false;
    sentShutDownSignal = false;
    model = new CES_Model();
}

/*
 * Type: public
 *
 * Initializes the GUI variable
 *
 * Parameters:
 * - The GUI created on initialization
 * */
void Controller::setGUI(CES_GUI* ui){
    gui = ui;
}

/*
 * Type: public
 *
 * Steps necessary to switch on the device:
 * 1- construct model
 * 2- enable buttons
 * 3- start timer
 * */
void Controller::startController(){
    qInfo("Contoller=> Booting the device up!");
    //model = new CES_Model();

    gui->enableOffScreen(false);
    gui->enableTimeButton(true);
    gui->enableWaveformButton(true);
    gui->enableFreqButton(true);
    gui->enableStartTreatmentButton(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer,SIGNAL(timeout()), model, SLOT(update()));
    timer->start(1000);

    deviceOn = true;
}

/*
 * Type: public
 *
 * Steps necessary to switch on the device:
 * 1- destruct model
 * 2- destruct timer
 * 3- disable screen
 * */
void Controller::endController(){
    //delete model;
    if(model->getTreatmentOngoing()){
        model->endTreatment();
    }
    model->setIdleTime(0);

    delete timer;
    gui->enableTimeButton(false);
    gui->enableWaveformButton(false);
    gui->enableFreqButton(false);
    gui->enableStartTreatmentButton(false);

    deviceOn = false;
    gui->enableOffScreen(true);
    gui->setRemainingTime((duration + 1) * 20 * 60);
}

/*
 * Type: public
 *
 * When the on/off power button is pressed:
 * - Start the controller if the device is off
 * - End the controller if the device is on
 * */
void Controller::powerButtonClicked(){
    if(deviceOn){
        this->endController();
    } else {
        this->startController();
    }
}

/*
 * Type: public
 *
 * Disables the treatment parameter buttons
 * Then begins the treatment using the current parameters selected by the user
 * */
void Controller::startTreatmentPressed(){
    gui->enableTimeButton(false);
    gui->enableWaveformButton(false);
    gui->enableFreqButton(false);
    gui->enableStartTreatmentButton(false);
    model->beginTreatment(waveform, frequency, duration, power);
}

/*
 * Type: public
 *
 * Increase the power by 1 unit if not already at max
 *
 * Set the power value in the GUI
 *
 * If there is a treatment ongoing inform the model of the increase
 * */
void Controller::increasePowerLevelPressed(){
    if(power < 9){
        power++;
    }

    gui->setPowerLevel(power);

    if(model->getTreatmentOngoing()){
        model->changePowerLevel(0);
    }
}

/*
 * Type: public
 *
 * Decrease the power by 2 units if not already at minimum
 *
 * Set the power value in the GUI
 *
 * If there is a treatment ongoing inform the model of the decrease
 * */
void Controller::decreasePowerLevelPressed(){
    int i = 0;
    while(power > 0 && i < 2){
        power--;
        i++;
    }

    gui->setPowerLevel(power);

    if(model->getTreatmentOngoing()){
        model->changePowerLevel(1);
    }
}

/*
 * Type: public
 *
 * Toggles whether the treatment will be saved and inform the model and the GUI of the change
 * */
void Controller::saveTreatmentPressed(){
    saveTreatment = !saveTreatment;
    model->setWillSave(saveTreatment);
    gui->toggleSaveTreatmentButton(!saveTreatment);
}

/*
 * Type: public
 *
 * Cycles between the duration options and updates the GUI
 * */
void Controller::cycleTimeOptionsButtonPressed(){
    duration = (duration + 1) % 3;
    gui->setTimeSetting(duration);
    gui->setRemainingTime((duration + 1) * 20 * 60);
}

/*
 * Type: public
 *
 * Cycles between the waveform options and updates the GUI
 * */
void Controller::cycleWaveformOptionsButtonPressed(){
    waveform = (waveform + 1) % 3;
    gui->setWaveform(waveform);
}

/*
 * Type: public
 *
 * Cycles between the frequency options and updates the GUI
 * */
void Controller::cycleFreqOptionsButtonPressed(){
    frequency = (frequency + 1) % 3;
    gui->setFrequency(frequency);
}

/*
 * Type: public
 *
 * Toggles whether the earlobe clips are connected and informs the model and the GUI
 * */
void Controller::connectEarlobeClipsPressed(){
    electrodesConnected = !electrodesConnected;
    model->toggleElectrodes();
    gui->toggleConnectEarlobeButtonText(!electrodesConnected);
    gui->enableElectrodesSymbol(electrodesConnected);
}

/*
 * Type: public
 *
 * Toggles whether the time is sped up informs the model and the GUI
 * */
void Controller::slowDownTimePressed(){
    timeSpedUp = !timeSpedUp;
    model->changeTimeScale();
    gui->toggleTimeScaleButtonText(timeSpedUp);
}

/*
 * Type: public
 *
 * Updates the Gui with the most recent information from the model
 *
 * Checks whether the device's battery is too low, and sends a shutdown signal if it is
 *
 * If a treatment is ongoing update the timer in the GUI,
 * otherwise ensure control buttons are enabled, and treatment history is up todate
 * */
void Controller::update(){
    qInfo("Controller=> Updating!");
    qDebug() << "Is treatment on going? = " << model->getTreatmentOngoing();
    gui->setBatteryLevel(model->getBatteryLevel());

//    //check if battery is dead.
    if(!sentShutDownSignal && model->getBatteryLevel() <= 2){
       sentShutDownSignal = true;
       gui->enableOnButton(false);
       QTimer::singleShot(5000, this, &Controller::shutDown);
    }

    //auto shut down after being idle for 30 mins
    int maxIdleTime = MAX_IDLE_TIME;
    if(model->getIdleTime() >= maxIdleTime){
        shutDown();
    }

    if(model->getTreatmentOngoing()){
       gui->setRemainingTime(model->getRemainSeconds());
    } else {
        gui->enableTimeButton(true);
        gui->enableWaveformButton(true);
        gui->enableFreqButton(true);
        gui->enableStartTreatmentButton(true);
        gui->updateTreatmentHistory(model->getTreatmentHist());
    }
}

/*
 * Type: public
 *
 * called when a power surge signal is sent, permanently disable the GUI
 * */
void Controller::powerSurgePressed(){
    qInfo("power surge detected! shutting down!");
    gui->enableOnButton(false);
    shutDown();
}

/*
 * Type: public
 *
 * Switch off the device
 * */
void Controller::shutDown(){
    endController();
}
