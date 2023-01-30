#ifndef CES_MODEL_H
#define CES_MODEL_H

#include <QObject>
#include <QDateTime>
#include <vector>
#include "electrodes.h"
#include "battery.h"
#include "treatments.h"
#include <QVector>

class CES_Model : public QObject
{
    Q_OBJECT

public:
    CES_Model();
    ~CES_Model();
    void beginTreatment(int, int, int, int);
    void endTreatment();
    void changePowerLevel(int);
    void toggleElectrodes();
    void setWillSave(bool);
    void setIdleTime(int);
    float getBatteryLevel();
    bool getTreatmentOngoing();
    int getRemainSeconds();
    int getIdleTime();
    QVector<treatments*>* getTreatmentHist();
    void changeTimeScale();

public slots:
    void update();

private:
    QVector<treatments*> treatmentHist;
    treatments* curTreatment;

    battery* curBattery;

    electrodes* curElectrodes;

    int remainSeconds;
    bool treatmentOngoing;
    bool willSave;
    int secsPerMin;
    int idleTime;
};

#endif // CES_MODEL_H
