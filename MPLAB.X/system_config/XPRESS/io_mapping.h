/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

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
#include "buttons.h"
#include "leds.h"
#include "power.h"

/*******************************************************************/
/******** USB stack hardware selection options *********************/
/*******************************************************************/
//This section is the set of definitions required by the MCHPFSUSB
//  framework.  These definitions tell the firmware what mode it is
//  running in, and where it can find the results to some information
//  that the stack needs.
//These definitions are required by every application developed with
//  this revision of the MCHPFSUSB framework.  Please review each
//  option carefully and determine which options are desired/required
//  for your application.

/** USB ************************************************************/
//The PIC18F87J50 FS USB Plug-In Module supports the USE_USB_BUS_SENSE_IO
//feature.  The USE_SELF_POWER_SENSE_IO feature is not implemented on the
//circuit board, so the USE_SELF_POWER_SENSE_IO define should always be
//commented for this hardware platform.

//#define USE_SELF_POWER_SENSE_IO
#define tris_self_power     TRISAbits.TRISA2    // Input
#if defined(USE_SELF_POWER_SENSE_IO)
#define self_power          PORTAbits.RA2
#else
#define self_power          1
#endif

//#define USE_USB_BUS_SENSE_IO		//JP1 must be in R-U position to use this feature on this board
#define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
#if defined(USE_USB_BUS_SENSE_IO)
#define USB_BUS_SENSE       PORTBbits.RB5
#else
#define USB_BUS_SENSE       1
#endif

#define VBUS_INPUT PORTAbits.RA5

//Uncomment this to make the output HEX of this project
//   to be able to be bootloaded using the HID bootloader
#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
