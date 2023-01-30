#ifndef BATTERY_H
#define BATTERY_H

//keep track of critical battery

class battery
{
public:
    battery(); //batter constructor
    bool drainBattery(); //called every update to drain the battery by the drain strength
    void setDrainStrength(float); //setter for the drain strength which is represented by a float
    float getLevel(); //getter for the current battery level, returns as an int
private:
    float batteryLevel; //represents the current battery charge as a float
    float drainStrength; //represents the current drain strength as a float
};

#endif // BATTERY_H
