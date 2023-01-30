#include "treatments.h"
#include <QDebug>

treatments::treatments()
{
    waveform = 1;
    frequency = 1;
    
    duration = 20;
    
    powerLevel = 150; 
    qInfo("Treatment created");
}

treatments::treatments(int w, int f, int d, int p) : waveform(w), frequency(f), duration(d), powerLevel(p) 
{
    startTime = new QDateTime(QDateTime::currentDateTime()); 

}

//####################SETTER & GETTERS##################################

void treatments::setPowerlevel(int l)
{
    powerLevel = l; //set the powerLevel to the given int by model
}

void treatments::setDuration(int d)
{
    duration = d; //set the duration to the given int by model
}

int treatments::getWaveform()
{
    return waveform; //return the waveform to the model
}

int treatments::getFrequency()
{
    return frequency; //return the frequency to the model
}

QDateTime* treatments::getStartTime()
{
    return startTime;
}

int treatments::getPowerLevel()
{
    return powerLevel; //return the powerLevel to the model
}

int treatments::getDuration(){
    return duration;
}
