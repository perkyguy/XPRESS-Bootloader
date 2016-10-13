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
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef LVP_H
#define	LVP_H

#define _XTAL_FREQ  48000000L

#define INPUT_PIN           1
#define OUTPUT_PIN          0

#define SLAVE_RUN           0
#define SLAVE_RESET         1

#define ICSP_TRIS_DAT       TRISBbits.TRISB3
#define ICSP_DAT            LATBbits.LATB3
#define ICSP_TRIS_CLK       TRISBbits.TRISB2
#define ICSP_CLK            LATBbits.LATB2
#define ICSP_TRIS_nMCLR     TRISBbits.TRISB4
#define ICSP_nMCLR          LATBbits.LATB4

#define  LVP_init() LVP_exit();

void LVP_enter( void);
void LVP_exit( void);
void LVP_addressLoad( uint16_t address);
void LVP_bulkErase( void);
void LVP_skip( uint16_t count);
bool LVP_inProgress(void);
void LVP_rowWrite( uint16_t *buffer, uint8_t n);
void LVP_cfgWrite( uint16_t *buffer, uint8_t n);

#endif	/* LVP_H */

