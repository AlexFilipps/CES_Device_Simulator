#ifndef TREATMENTS_H
#define TREATMENTS_H
#include <QDateTime>
//#include <QTime>

class treatments
{

public:
    //constructors
    treatments();
    treatments(int, int, int, int); //constructor used by model for creating a new treatment, sets all treatment properties

    //setters
    void setPowerlevel(int); //setter for powerlevel
    void setDuration(int); //setter for duration

    //getters
    int getWaveform(); // getter for waveform
    int getFrequency(); //getter for frequency
    QDateTime* getStartTime(); //getter for start time
    int getPowerLevel(); //getter for powerlevel
    int getDuration(); //getter for duration
    
private:
    int waveform; //waveformt the freqeuncy was run on
    int frequency; //frequency the treatment was run on
    QDateTime* startTime; //start time of the treatment
    int duration; //duration of treatment
    int powerLevel; //int 0-9, to convert to power level for display; powerLevel * 50 + 50

};

#endif // TREATMENTS_H
