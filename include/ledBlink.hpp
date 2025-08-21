#include <Arduino.h>

bool ioBlink = false;
unsigned long milOld;
int rndTemp = 0 ;


void blinkSingle(int speed, int _pin){
    if ((milOld + speed) < millis   ())
    {
        milOld = millis();
        if (ioBlink == false)
        {
            digitalWrite(_pin, HIGH);
            ioBlink = true;
        }else{
            digitalWrite(_pin, LOW);
            ioBlink = false;
        }       
    }
}


//  Conmutación Asíncrona

void blinkSingleAsy(int timeHigh, int timeLow, int _pin) {
    if (!ioBlink) {
        if ((milOld + timeHigh) < millis()) {
            milOld = millis();
            digitalWrite(_pin, LOW);
            ioBlink = true;
        }
    } else {
        if ((milOld + timeLow) < millis()) {
            milOld = millis();
            digitalWrite(_pin, HIGH);
            ioBlink = false;
        }
    }
}

// Conmutación Random

void blinkRandomSingle(int minTime, int maxTime, int _pin){
    if ((milOld + rndTemp) < millis())
    {
        milOld = millis();
        rndTemp = random(minTime, maxTime);
        if (ioBlink == false)
        {
            digitalWrite(_pin, HIGH);
            ioBlink = true;
        }else{
            digitalWrite(_pin, LOW);
            ioBlink = false;
        }
        
    }
    
}


// On

void setOnSingle(int _pin){
    digitalWrite(_pin, HIGH);
}


// Off

void setOffSingle(int _pin){
    digitalWrite(_pin, LOW);
}


/*
    Conmutación LED MQTT Transmisión
*/

void mqttTX(){
    for (int i = 0; i < 2; i++)
    {
        setOnSingle(MQTTLED);
        delay(50);
        setOffSingle(MQTTLED);
        delay(10);
    }
    
}
/*
    Conmutación de LED MQTT Recepción
}*/
void mqttRX(){

    for (int i = 0; i < 1; i++)
    {
        blinkRandomSingle(5, 50, MQTTLED);
        delay(50);
    }
      

}



// On/Off Status 

void setOnOff(int _pin, boolean status){
    digitalWrite(_pin, status ? HIGH : LOW);

}
