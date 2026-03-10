#include <stdio.h>
#include "myGPIO.h"
#include "myADC.h"

#define KNOP_PIN 2
#define LED_PIN 1
#define pot 0

void main() 
{

}

    int getal = myADC_getValue(pot)*10/1074;

    if (myGPIO_KNOP_GetFlank(KNOP_PIN)){
        
    }
    
}