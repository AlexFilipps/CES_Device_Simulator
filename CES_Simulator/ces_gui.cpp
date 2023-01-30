#include "ces_gui.h"
#include "ui_ces_gui.h"
#include "treatments.h"

#include <QDir>
#include <QDebug>
#include <QTimer>

CES_GUI::CES_GUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CES_GUI)
{
    ui->setupUi(this);

    numHistoryEntriesBeingDisplayed = 0;
    haveDisplayedLowBatteryWarning = false;
    haveDisplayedDeadBatteryWarning = false;

    //initiallize pictures.
    QString path = QDir::currentPath();
    path.remove("build-CES_Simulator-Desktop-Debug");
    path.append("COMP3004-Project/");
    path.append("earlobes.png");
    QPixmap pix(path);
    ui->earlobesConnectedPic->setPixmap(pix);

    QString path2 = QDir::currentPath();
    path2.remove("build-CES_Simulator-Desktop-Debug");
    path2.append("COMP3004-Project/");
    path2.append("rsz_earlobes_crossed_out.jpg");
    QPixmap pix2(path2);
    ui->earlobesDisconnectedPic->setPixmap(pix2);

    powerLevelBarNotches.push_back(ui->powerBar1);
    powerLevelBarNotches.push_back(ui->powerBar2);
    powerLevelBarNotches.push_back(ui->powerBar3);
    powerLevelBarNotches.push_back(ui->powerBar4);
    powerLevelBarNotches.push_back(ui->powerBar5);
    powerLevelBarNotches.push_back(ui->powerBar6);
    powerLevelBarNotches.push_back(ui->powerBar7);
    powerLevelBarNotches.push_back(ui->powerBar8);
    powerLevelBarNotches.push_back(ui->powerBar9);
    powerLevelBarNotches.push_back(ui->powerBar10);

    //connect buttons to function calls
    connect(ui->startTreatmentButton, SIGNAL(clicked()), &controller, SLOT(startTreatmentPressed()));
    connect(ui->increasePowerButton, SIGNAL(clicked()), &controller, SLOT(increasePowerLevelPressed()));
    connect(ui->decreasePowerButton, SIGNAL(clicked()), &controller, SLOT(decreasePowerLevelPressed()));
    connect(ui->saveTreatmentButton, SIGNAL(clicked()), &controller, SLOT(saveTreatmentPressed()));
    connect(ui->timeButton, SIGNAL(clicked()), &controller, SLOT(cycleTimeOptionsButtonPressed()));
    connect(ui->wavefromButton, SIGNAL(clicked()), &controller, SLOT(cycleWaveformOptionsButtonPressed()));
    connect(ui->frequencyButton, SIGNAL(clicked()), &controller, SLOT(cycleFreqOptionsButtonPressed()));
    connect(ui->earlobeButton, SIGNAL(clicked()), &controller, SLOT(connectEarlobeClipsPressed()));
    connect(ui->changeTimeSpeedButton, SIGNAL(clicked()), &controller, SLOT(slowDownTimePressed()));
    connect(ui->triggerExcessPowerButton, SIGNAL(clicked()), &controller, SLOT(powerSurgePressed()));
    connect(ui->onButton, SIGNAL(clicked()), &controller, SLOT(powerButtonClicked()));
    connect(ui->offButton, SIGNAL(clicked()), &controller, SLOT(powerButtonClicked()));

    //set up scroll area for treatment history
    QWidget *newWidget = new QWidget();
    this->vertScrollArea = new QVBoxLayout(newWidget);
    ui->savedTreatmentsArea->setWidget(newWidget);
    ui->savedTreatmentsArea->setWidgetResizable(true);

    /* Test for treatment history*/
//    QVector<treatments*> treatmentsExample;
//    treatmentsExample.append(new treatments(0,0,225,0));
//    treatmentsExample.append(new treatments(1,1,500,9));
//    treatmentsExample.append(new treatments(1,1,3600,3));
//    treatmentsExample.append(new treatments(1,1,3700,3));
//    treatmentsExample.append(new treatments(1,1,10,3));
//    treatmentsExample.append(new treatments(1,1,120,3));
//    treatmentsExample.append(new treatments(1,1,60,3));
//    treatmentsExample.append(new treatments(1,1,500,3));
//    treatmentsExample.append(new treatments(1,1,500,3));
//    treatmentsExample.append(new treatments(1,1,500,3));
//    treatmentsExample.append(new treatments(1,1,500,3));
//    treatmentsExample.append(new treatments(1,1,500,3));
//    updateTreatmentHistory(&treatmentsExample);
    /* Test for treatment history*/


    enableElectrodesSymbol(false);
    enableOffScreen(true);

    toggleTimeScaleButtonText(false);
    ui->lowBatteryWarning->setVisible(false);
    ui->deadBatteryWarning->setVisible(false);


    controller.setGUI(this);
}

CES_GUI::~CES_GUI()
{
    delete ui;
}

//from 0-100
void CES_GUI::setBatteryLevel(float batteryLevel){
    float percentage = (float)batteryLevel / 100.0f;
    int maxLen = FULL_BATTERY_LEN;
    int startingYPos = STARTING_BATTERY_Y;
    int newLen = (int)(percentage * maxLen);
    if(newLen < 0) { newLen = 0; }
    if(newLen > maxLen) { newLen = maxLen; }
    ui->batteryForeground->setFixedHeight(newLen);
    ui->batteryForeground->move(ui->batteryForeground->pos().x(),
                                (startingYPos + (maxLen-newLen)));

    if(!haveDisplayedLowBatteryWarning && percentage <= 0.05f){
        haveDisplayedLowBatteryWarning = true;
        ui->lowBatteryWarning->setVisible(true);
        QTimer::singleShot(5000, this, &CES_GUI::hideLowBatteryWarning);
    }

    if(!haveDisplayedDeadBatteryWarning && percentage <= 0.02f){
        haveDisplayedDeadBatteryWarning = true;
        ui->deadBatteryWarning->setVisible(true);
        QTimer::singleShot(5000, this, &CES_GUI::hideDeadBatteryWarning);
    }
}

void CES_GUI::hideLowBatteryWarning(){
    ui->lowBatteryWarning->setVisible(false);
}

void CES_GUI::hideDeadBatteryWarning(){
    ui->deadBatteryWarning->setVisible(false);
}

//time in seconds, max = 1 hr.
void CES_GUI::setRemainingTime(int timeInSeconds){
    int numMins = timeInSeconds / 60;
    int numSecs = timeInSeconds - (numMins * 60);
//    qDebug() << "timeInSeconds = " << timeInSeconds << ", numMins=" << numMins
//             << ", numSecs=" << numSecs;
    std::string newTimeText = std::to_string(numMins) + ":";
    if(numSecs <= 9 && numSecs > 0){
        newTimeText.append("0" + std::to_string(numSecs));
    }else if(numSecs <= 0){
        newTimeText.append("00");
    }else{
        newTimeText.append(std::to_string(numSecs));
    }

    QString qstr = QString::fromStdString(newTimeText);
    ui->time->setText(qstr);
}

//true = enable, false = disable.
void CES_GUI::enableElectrodesSymbol(bool electrodeSymbolEnabled){
    ui->earlobesConnectedPic->setVisible(electrodeSymbolEnabled);
    ui->earlobesDisconnectedPic->setVisible(!electrodeSymbolEnabled);
}

//int from 0-9
void CES_GUI::setPowerLevel(int powerLevel){
    for(int i = 0; i < 10; i++){
        if(i <= powerLevel){
            powerLevelBarNotches[i]->setStyleSheet("background-color: rgb(0,0,0)");
        }else{
            powerLevelBarNotches[i]->setStyleSheet("background-color: rgb(255,255,255)");
        }
    }
}

//true = enable, false = disable.
void CES_GUI::enableStartTreatmentButton(bool shouldEnableButton){
    if(shouldEnableButton){
        ui->startTreatmentButton->setStyleSheet("background-color: rgb(173,216,230)");
        ui->startTreatmentButton->setEnabled(true);
    }else{
        ui->startTreatmentButton->setStyleSheet("background-color: rgb(255,255,255)");
        ui->startTreatmentButton->setEnabled(false);
    }
}

//true = "save treatment", false = "do not save treatment".
void CES_GUI::toggleSaveTreatmentButton(bool shouldSetToSaveText){
    if(shouldSetToSaveText){
        ui->saveTreatmentButton->setStyleSheet("background-color: rgb(255,255,0)");
        ui->saveTreatmentButton->setText("Save\n Treatment");
    }else{
        ui->saveTreatmentButton->setStyleSheet("background-color: rgb(255,0,0)");
        ui->saveTreatmentButton->setText("Do NOT\n Save\n Treatment");
    }
}

//int represents id, 0 = 20 mins, 1=40mins, 2=60mins
void CES_GUI::setTimeSetting(int timeId){
    ui->timeOption1->setStyleSheet("background-color: rgb(255,255,255)");
    ui->timeOption2->setStyleSheet("background-color: rgb(255,255,255)");
    ui->timeOption3->setStyleSheet("background-color: rgb(255,255,255)");

    if(timeId == 0){
        ui->timeOption1->setStyleSheet("background-color: rgb(255,255,0)");
    }else if(timeId == 1){
        ui->timeOption2->setStyleSheet("background-color: rgb(255,255,0)");
    }else{
        ui->timeOption3->setStyleSheet("background-color: rgb(255,255,0)");
    }
}

//int represents id, 0=alpha, 1=beta, 2=gamma
void CES_GUI::setWaveform(int waveFormId){
    ui->waveform1->setStyleSheet("background-color: rgb(255,255,255)");
    ui->waveform2->setStyleSheet("background-color: rgb(255,255,255)");
    ui->waveform3->setStyleSheet("background-color: rgb(255,255,255)");

    if(waveFormId == 0){
        ui->waveform1->setStyleSheet("background-color: rgb(255,255,0)");
    }else if(waveFormId == 1){
        ui->waveform2->setStyleSheet("background-color: rgb(255,255,0)");
    }else{
        ui->waveform3->setStyleSheet("background-color: rgb(255,255,0)");
    }
}

//int represents id, 0=0.5hz, 1=77hz, 2=100hz
void CES_GUI::setFrequency(int freqId){
    ui->frequency1->setStyleSheet("background-color: rgb(255,255,255)");
    ui->frequency2->setStyleSheet("background-color: rgb(255,255,255)");
    ui->frequency3->setStyleSheet("background-color: rgb(255,255,255)");

    if(freqId == 0){
        ui->frequency1->setStyleSheet("background-color: rgb(255,255,0)");
    }else if(freqId == 1){
        ui->frequency2->setStyleSheet("background-color: rgb(255,255,0)");
    }else{
        ui->frequency3->setStyleSheet("background-color: rgb(255,255,0)");
    }
}

//pointer to vector with treatments
void CES_GUI::updateTreatmentHistory(QVector<treatments*>* treatments){
    if(treatments->length() <= numHistoryEntriesBeingDisplayed){
        return;
    }

    int numEntriesToDisplay = treatments->length() - numHistoryEntriesBeingDisplayed;

    for(int i = treatments->length() - numEntriesToDisplay; i < treatments->length(); i++){
        addTreatmentToHistory((*treatments)[i]);
        numHistoryEntriesBeingDisplayed++;
    }
}

void CES_GUI::addTreatmentToHistory(treatments* treatment){
    //create box for new treatment history entry
    QVBoxLayout *newTreatmentWidget = new QVBoxLayout();
    QLabel *newTreatmentEntry = new QLabel();
    newTreatmentEntry->setStyleSheet("border: 2px solid black;");
    newTreatmentWidget->insertWidget(0,newTreatmentEntry);
    this->vertScrollArea->insertLayout(numHistoryEntriesBeingDisplayed,
                                       newTreatmentWidget);

    //create the text for the entry.
    QString entryText;

    //add date stamp
    entryText.append("Started: ");
    entryText.append(treatment->getStartTime()->toString());
    entryText.append("\n");

    //add waveform
    entryText.append("Waveform: ");
    if(treatment->getWaveform() == 0){
        entryText.append("Alpha\n");
    }else if(treatment->getWaveform() == 1){
        entryText.append("Beta\n");
    }else if(treatment->getWaveform() == 2){
        entryText.append("Gamma\n");
    }else{
        entryText.append("MISSING WAVEFORM\n");
    }

    //add frequency
    entryText.append("Frequency: ");
    if(treatment->getFrequency() == 0){
        entryText.append("0.5 hz\n");
    }else if(treatment->getFrequency() == 1){
        entryText.append("77 hz\n");
    }else if(treatment->getFrequency() == 2){
        entryText.append("100 hz\n");
    }else{
        entryText.append("MISSING FREQUENCY\n");
    }

    //add duration
    entryText.append("Duration: ");
    //check for time >= 1 hour
    if(treatment->getDuration() >= 3600){
        entryText.append("1 hour");
    }else{
        int numMins = treatment->getDuration() / 60;
        int numSecs = treatment->getDuration() - (numMins * 60);

        if(numMins > 0){
            entryText.append(QString::number(numMins));
            entryText.append(" mins ");
        }

        if(numSecs > 0){
            entryText.append(QString::number(numSecs));
            entryText.append(" secs ");
        }
    }

    //add power level
    entryText.append("\nPower Level: ");
    entryText.append(QString::number((treatment->getPowerLevel() * 50)+ 50));
    entryText.append(" microamperes");

    newTreatmentEntry->setText(entryText);
}

//true=>sets button's text to "connect...", false=> set's button to "disconnect..."
void CES_GUI::toggleConnectEarlobeButtonText(bool shouldSetTextToConnected){
    if(shouldSetTextToConnected){
        ui->earlobeButton->setText("Connect Earlobe\n Clips");
    }else{
        ui->earlobeButton->setText("Disconnect Earlobe\n Clips");
    }
}

//true=>sets button's text to "slow down time", false=> set's button to "speed up time"
void CES_GUI::toggleTimeScaleButtonText(bool shouldSetTextToSlowTime){
    if(shouldSetTextToSlowTime){
        ui->changeTimeSpeedButton->setText("Slow Down\n Time");
    }else{
        ui->changeTimeSpeedButton->setText("Speed Up\n Time");
    }
}

//true=>sets time button enabled, false=>sets time button disabled.
void CES_GUI::enableTimeButton(bool shouldEnableTimeButton){
    ui->timeButton->setEnabled(shouldEnableTimeButton);
}

//true=>sets waveform button enabled, false=>sets waveform button disabled.
void CES_GUI::enableWaveformButton(bool shouldEnableWaveformButton){
    ui->wavefromButton->setEnabled(shouldEnableWaveformButton);
}

//true=>sets freq button enabled, false=>sets freq button disabled.
void CES_GUI::enableFreqButton(bool shouldEnableFreqButton){
    ui->frequencyButton->setEnabled(shouldEnableFreqButton);
}

//true=>turns ON the off screen, false=>turns OFF the off screen (normal screen is on).
void CES_GUI::enableOffScreen(bool shouldEnableOffScreen){
    ui->offScreenBackground->setVisible(shouldEnableOffScreen);
    ui->offScreenLabel->setVisible(shouldEnableOffScreen);
    ui->onButton->setVisible(shouldEnableOffScreen);
    ui->earlobeButton->setEnabled(!shouldEnableOffScreen);
    ui->changeTimeSpeedButton->setEnabled(!shouldEnableOffScreen);
    ui->triggerExcessPowerButton->setEnabled(!shouldEnableOffScreen);
}

//true=>sets on button enabled, false=>sets on button disabled.
void CES_GUI::enableOnButton(bool shouldEnableOnButton){
    ui->onButton->setEnabled(shouldEnableOnButton);
}
