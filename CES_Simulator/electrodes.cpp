#include "electrodes.h"
#include <QDebug>
electrodes::electrodes()
{
    attachedToSkin = false; //set to false as default
    failedTreatment = false; //set to false as default
    counter = 0; //set to 0 as the treatment should no fail before starting
    qInfo("elect");
}

/*
 * Type: Public
 * Toggles the contact status of the electrodes. true for attached, false for detached
 *
 * Parameters: None
 * 
 *
 * Returns: void
 */

void electrodes::toggleContactStatus()
{
    if(attachedToSkin == true){ //check if the electrodes are connected, if they are set to false
        attachedToSkin = false;

    } else{ //if electrodes are not connected, set to true
        attachedToSkin = true;
        counter = 0; //reset counter to 0 as the electrodes are now connected
    }
}

//####################SETTER & GETTERS##################################

bool electrodes::getStatus()
{
    if(attachedToSkin == false){ //if the electrodes are disconnected trigger counter to count up

        counter++; 
        
        if(counter >= 5){ //if the counter is higher than 5 seconds set the treatment failed to true
            failedTreatment = true;
        } else {
            failedTreatment = false;
        }
    }
    return attachedToSkin; //return to the model the status of the electrodes
}

bool electrodes::getFailed(){return failedTreatment;} //return to the model the status of failedTreatment
