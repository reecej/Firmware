/*This file is part of the Maslow Control Software.
    The Maslow Control Software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Maslow Control Software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with the Maslow Control Software.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2014-2017 Bar Smith*/

// This contains all of the Spindle commands

#include "Maslow.h"

// the variable SpindlePowerControlPin is assigned in configAuxLow() in System.cpp 

// Globals for Spindle control, both poorly named
Servo myservo;  // create servo object to control a servo

void  setSpindlePower(bool powerState) {
    /*
     * Turn spindle on or off depending on powerState
     */
    boolean useServo = !sysSettings.spindleAutomate;
    boolean activeHigh = true;
    int delayAfterChange = 1000;  // milliseconds
    int servoIdle =  90;  // degrees
    int servoOn   = 180;  // degrees
    int servoOff  =   0;  // degrees
    int servoDelay = 2000;  // milliseconds

    // Now for the main code
    #if defined (verboseDebug) && verboseDebug > 1
    Serial.print(F("Spindle control uses pin "));
    Serial.print(SpindlePowerControlPin);
    #endif
    if (useServo) {   // use a servo to control a standard wall switch
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" with servo (idle="));
        Serial.print(servoIdle);
        Serial.print(F(", on="));
        Serial.print(servoOn);
        Serial.print(F(", off="));
        Serial.print(servoOff);
        Serial.println(F(")"));
        #endif
        myservo.attach(SpindlePowerControlPin); // start servo control
        myservo.write(servoIdle);   // move servo to idle position
        maslowDelay(servoDelay);    // wait for move to complete
        if(sys.stop){return;}
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            myservo.write(servoOn); // move servo to turn on switch
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            myservo.write(servoOff); // move servo to turn off switch
        }
        maslowDelay(servoDelay);    // wait for move to complete
        if(sys.stop){return;}
        myservo.write(servoIdle);   // return servo to idle position
        maslowDelay(servoDelay);    // wait for move to complete
        if(sys.stop){return;}
        myservo.detach();           // stop servo control
    }
    else {            // use a digital I/O pin to control a relay
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" as digital output, active "));
        if (activeHigh) Serial.println(F("high"));
        else Serial.println(F("low"));
        #endif
        pinMode(SpindlePowerControlPin, OUTPUT);
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            if (activeHigh) digitalWrite(SpindlePowerControlPin, HIGH);
            else digitalWrite(SpindlePowerControlPin, LOW);
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            if (activeHigh) digitalWrite(SpindlePowerControlPin, LOW);
            else digitalWrite(SpindlePowerControlPin, HIGH);
        }
    }
    maslowDelay(delayAfterChange);
}
