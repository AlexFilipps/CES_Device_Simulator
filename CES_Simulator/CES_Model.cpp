#include "CES_Model.h"
#include <QDebug>

CES_Model::CES_Model(){
    remainSeconds = 0;
    treatmentOngoing = false;
    willSave = false;
    secsPerMin = 60;
    idleTime = 0;

    curTreatment = nullptr;
    curBattery = new battery();
    curElectrodes = new electrodes();
}

CES_Model::~CES_Model(){
    treatmentHist.clear();

    if(curTreatment = nullptr){
        delete(curTreatment);
    }

    delete(curBattery);
    delete(curElectrodes);
}


/*
 * Type: Public
 * Advances time on any ongoing treatment, and updates other components of the model as needed
 *
 * Parameters: None
 *
 * Returns: Void
 */
void CES_Model::update(){
    qDebug() << "Model=> Update function called!";

    //check if no treatment is happening
    if (!treatmentOngoing){
        idleTime += 60 / secsPerMin;
        qDebug() << "Model=> No on-going treatment was detected, aborting!";
        return;
    }
    idleTime = 0;

    //check if electrodes are connected and we can continue treatment
    if (curElectrodes->getStatus()){
        qDebug() << "Model=> Electrodes detected as connected!";
        if (remainSeconds <= 0){                //treatment has finished
            qDebug() << "Model=> Time ran out in treatment! Ending treatment now!";
            endTreatment();
            return;
        }
        else {                                  //continue treatment
            qDebug() << "Model=> Decrementing treatment time!";
            remainSeconds -= 60 / secsPerMin;
            if (curBattery->drainBattery()){   //battery is out of power and we need to shut down
                qDebug() << "Model=> Battery detected as dead! Shutting down soon.";
                return; //do nothing
            }
        }
    }
    else { //electrodes are not connected and we need to check if treatment has terminated
        qDebug() << "Model=> Earlobes detected as NOT connected. Handling this now...";
        if (curElectrodes->getFailed()){        //treatment was paused for too long and has failed
            qDebug() << "Model=> Earlobes disconnected for longer than 5 seconds. Ending treatment!";
            endTreatment();
            return;
        }
        else {                                   //treatment is currently paused
            qDebug() << "Model=> Earlobes detected, waiting for them to reconnect!";
            return;
            //do nothing
        }
    }
}



/*
 * Type: Public
 * Instantiates a new treatment with the given parameters
 *
 * Parameters:
 * wave - an int ID representing the waveform of the treatment
 * freq - an int ID representing the frequency of the wave
 * dur - an int ID representing how long the treatment should be
 * power - an int ID representing the power level that the treatment should start at
 *
 * Returns: Void
 */
void CES_Model::beginTreatment(int wave, int freq, int dur, int power){
    int realDuration = 0; //store the requested duration of the treatment in seconds
    if (dur == 0){
        realDuration = 1200;
    }
    else if (dur == 1){
        realDuration = 2400;
    }
    else if (dur == 2){
        realDuration = 3600;
    }

    remainSeconds = realDuration;
    curTreatment = new treatments(wave, freq, realDuration, power);
    treatmentOngoing = true;
    curBattery->setDrainStrength((0.5 + (curTreatment->getPowerLevel() * 0.05)) / secsPerMin);
}


/*
 *Type: Public
 * Ends a treatment, and saves it to history if the user has requested it
 *
 * Parameters: None
 *
 * Returns: Void
 */
void CES_Model::endTreatment(){
    qInfo("end treatment called!");
    //save the treatment to the history
    if (willSave){

        if(remainSeconds < 0){
            remainSeconds = 0;
        }

        //set the duration to be the length of time the treatment actually ran for
        curTreatment->setDuration(curTreatment->getDuration() - remainSeconds);
        treatmentHist.push_back(curTreatment);
    }
    treatmentOngoing = false;
    idleTime = 0;

}


/*
 * Type: Public
 * Increments/Decrements the power level of the currently running treatment, and update the rest of the model to the change
 *
 * Paramerters:
 * changeID - an int ID representing how the power should be changed. 0 means an increase, and 1 means a decrease
 *
 * Returns: Void
 */
void CES_Model::changePowerLevel(int changeID){
    if ((changeID == 0) && (curTreatment->getPowerLevel() < 9)){
        //INCREMENT THE POWER OF THE TREATMENT BY 1

        curTreatment->setPowerlevel(curTreatment->getPowerLevel() + 1);
    }
    else if ((changeID == 1) && (curTreatment->getPowerLevel() > 0)){
        if (curTreatment->getPowerLevel() == 1){
            //DECREMENT THE POWER OF THE TREATMENT BY 1

            curTreatment->setPowerlevel(curTreatment->getPowerLevel() - 1);
        }
        else{
            //DECREMENT THE POWER OF THE TREATMENT BY 2

            curTreatment->setPowerlevel(curTreatment->getPowerLevel() - 2);
        }
    }
    //update the battery drain speed to simulate our current power level
    curBattery->setDrainStrength((0.5 + (curTreatment->getPowerLevel() * 0.05)) / secsPerMin);
    qDebug() << "\nChanged the power level! Curr power level is now = " << curTreatment->getPowerLevel();
}


/*
 * Type: Public
 * Toggles the connection status of the electrodes
 *
 * Parameters: None
 *
 * Returns: Void
 */
void CES_Model::toggleElectrodes(){
    curElectrodes->toggleContactStatus();
}


/*
 * Type: Public
 * Toggles the speed at which time passes for the device. Alternates between 1 second = 1 second, and 1 second = 1 minute
 *
 * Paramerters: None
 *
 * Returns: Void
 */
void CES_Model::changeTimeScale(){
    //Swap the time scale between 1sec = 1min and 1sec = 1sec
    if (secsPerMin == 1){
        secsPerMin = 60;
    }
    else if (secsPerMin == 60){
        secsPerMin = 1;
    }

    //check if there is an ongoing treatment
    if(curTreatment == nullptr){
        return;
    }

    //update the battery drain speed
    curBattery->setDrainStrength((0.5 + (curTreatment->getPowerLevel() * 0.05)) / secsPerMin);
}



//####################SETTER & GETTERS##################################

void CES_Model::setWillSave(bool b){
    willSave = b;
}

void CES_Model::setIdleTime(int t){
    idleTime = t;
}

float CES_Model::getBatteryLevel(){
    return curBattery->getLevel();
}

QVector<treatments*>* CES_Model::getTreatmentHist(){
    return &treatmentHist;
}

bool CES_Model::getTreatmentOngoing(){
    return treatmentOngoing;
}

int CES_Model::getRemainSeconds(){
    return remainSeconds;
}

int CES_Model::getIdleTime(){
    return idleTime;
}
