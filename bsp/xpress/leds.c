/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

 bsp/leds.c 
 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

#include <leds.h>
#include <stdbool.h>
#include <xc.h>

#define LED_D1_LAT      LATCbits.LATC2
#define LED_D2_LAT      LATCbits.LATC2

#define LED_D1_TRIS     TRISCbits.TRISC2
#define LED_D2_TRIS     TRISCbits.TRISC2

//#define LED_D1_ANSEL    ANSELCbits.ANSC2
#define LED_D2_ANSEL    TRISCbits.TRISC2

#define LED_ON  0
#define LED_OFF 1

#define PIN_INPUT           1
#define PIN_OUTPUT          0

#define PIN_DIGITAL         0
#define PIN_ANALOG          1

/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_On(LED led)
{
    switch(led)
    {
//        case LED_D1:
//            LED_D1_LAT = LED_ON;
//            break;

        case LED_D2:
            LED_D2_LAT = LED_ON;
            break;
			
        case LED_NONE:
            break;
    }
}

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Off(LED led)
{
    switch(led)
    {
//        case LED_D1:
//            LED_D1_LAT = LED_OFF;
//            break;

        case LED_D2:
            LED_D2_LAT = LED_OFF;
            break;
			
        case LED_NONE:
            break;
    }
}

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Toggle(LED led)
{
    switch(led)
    {
//        case LED_D1:
//            LED_D1_LAT ^= 1;
//            break;

        case LED_D2:
            LED_D2_LAT ^= 1;
            break;
			
        case LED_NONE:
            break;
    }
}

/*********************************************************************
* Function: bool LED_Get(LED led);
*
* Overview: Returns the current state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Get(LED_CONNECTION_DETECTED);
*
* Output: true if on, false if off
*
********************************************************************/
bool LED_Get(LED led)
{
    switch(led)
    {
//        case LED_D1:
//            return ( (LED_D1_LAT == LED_ON) ? true : false );

        case LED_D2:
            return ( (LED_D2_LAT == LED_ON) ? true : false );
			
        case LED_NONE:
            return false;
    }
    
    return false;
}

/*********************************************************************
* Function: void LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void LED_Enable(LED led)
{
    switch(led)
    {
//        case LED_D1:
//            LED_D1_TRIS = PIN_OUTPUT;
//            LED_D1_ANSEL = PIN_DIGITAL;
//            break;

        case LED_D2:
            LED_D2_TRIS = PIN_OUTPUT;
            LED_D2_ANSEL = PIN_DIGITAL;
            break;
			
        case LED_NONE:
            break;
    }
}
