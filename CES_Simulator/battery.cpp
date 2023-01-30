#include "battery.h"
#include <QDebug>
battery::battery()
{
    batteryLevel = 100;
    drainStrength = 1; //Check what value should be default
    qInfo("battery");
}

/*
 * Type: Public
 * Drains the battery by the currently specified drain strength
 *
 * Parameters: None
 *
 * Returns: bool
 */

bool battery::drainBattery(){
    batteryLevel = batteryLevel - drainStrength; //decrease the battery charge by the current drain strength

    if (batteryLevel <= 2){
        return true;
    }
    return false;
}

/*
 * Type: Public
 * A setter for setting the current drain strength of the battery
 *
 * Parameters: 
 * s - a float representing the new drain strength
 *
 * Returns: void
 */

void battery::setDrainStrength(float s){
    drainStrength = s;  //set the current drain strength according to the given value from the model

}

/*
 * Type: Public
 * A getter for getting the current battery level of the battery
 *
 * Parameters: None
 * 
 *
 * Returns: 
 * batteryLevel - a float representing the current battery level
 */

float battery::getLevel(){return batteryLevel;} //return the current battery level to the model
