#ifndef ELECTRODES_H
#define ELECTRODES_H


class electrodes
{
public:
    electrodes(); //constructor for electrodes
    void toggleContactStatus(); //start a counter for attached to skin if disconnected, toggles attachedToSkin boolean
    bool getStatus(); //getter for current attached electrode status, i.e the bool for attachedToSkin
    bool getFailed(); //getter for failedTreatment boolean, if true: the treatment has failed and should be ended
private:
    bool attachedToSkin; //determines if the electrodes are attached to the patient
    bool failedTreatment; //determines if the treatment has failed
    int counter; //counts the iterations that have past since the electrodes that have discounted, if past 5 seconds, the failedTreatment boolean should become true
};

#endif // ELECTRODES_H
