#ifndef SLIDER_H
#define SLIDER_H
#include "Arduino.h"
#include "Stepper.h"

class Slider : Stepper {
    int currentPositionSteps = 0;
    int stepsPerMilimeter = 10;
    int distanceMilimeters = 100;
    int calibrationPinBegining = 0;
    int calibrationPinEnd = 0;
    public:
    using Stepper::Stepper;
    using Stepper::setSpeed;
    using Stepper::version;
    int currentPosition = 0;
    void step(int steps_to_move) 
    {
        Stepper::step(steps_to_move);
        currentPositionSteps += steps_to_move;
        calculateCurrentPosition();
    }
    

    void setCalibrationPins(const int BeginingPin,const int EndPin)
    {
        pinMode(calibrationPinBegining,INPUT_PULLUP);
        pinMode(calibrationPinEnd,INPUT_PULLUP);
        calibrationPinBegining = BeginingPin;
        calibrationPinBegining = EndPin;
    }
     
    bool moveToPosition_NonBlocking(const int newPosition)
    {
            int stepsLeft = calculateSteps(newPosition - currentPosition);  
            if(stepsLeft >0)
            {
                step(1);
            }        
            else if (stepsLeft <0)
            {
                step (-1);
            }else
            {
                return true;
            }
            
            return false;            
    }
    void moveToPosition_Blocking(const int newPosition)
    {   
        step(calculateSteps(newPosition - currentPosition));
    }
    int calculateSteps(const int distance)
    {
        return distance * stepsPerMilimeter;
    }
    void calculateCurrentPosition()
    {
        currentPosition = currentPositionSteps/stepsPerMilimeter;
    }
    void calibrate()
    {
        if(calibrationPinBegining!=0 && calibrationPinEnd !=0)
        {
            while(digitalRead(calibrationPinBegining)!=LOW)
            {
                step(-1);
            }
            currentPosition = 0;
            currentPositionSteps = 0;
            while(digitalRead(calibrationPinEnd)!=LOW)
            {
                step(1);
                currentPositionSteps++;
            }
            stepsPerMilimeter = distanceMilimeters/currentPositionSteps;
        }

        
    }
};


#endif // !SLIDER_H

