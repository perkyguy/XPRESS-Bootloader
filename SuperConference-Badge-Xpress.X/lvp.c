/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

 Low Voltage Programming Interface 
 
  Bit-Banged implementation of the PIC16F1 (250K) LVP protocol
  Based on the PIC16F188XX specification
  
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

#include "lvp.h"
#include "leds.h"

#define  CMD_LOAD_ADDRESS     0x80
#define  CMD_LATCH_DATA       0x00
#define  CMD_LATCH_DATA_IA    0x02
#define  CMD_INC_ADDR         0xF8
#define  CMD_BEGIN_PROG       0xE0
#define  CMD_BULK_ERASE       0x18


void ICSP_Init(void )
{
    ICSP_TRIS_DAT  = INPUT_PIN;
    ICSP_CLK       = 0;
    ICSP_TRIS_CLK  = OUTPUT_PIN;
    ICSP_nMCLR = SLAVE_RUN;
    ICSP_TRIS_nMCLR = OUTPUT_PIN;
}

void ICSP_Release( void)
{
    ICSP_TRIS_DAT  = INPUT_PIN;
    ICSP_TRIS_CLK  = INPUT_PIN;
    ICSP_nMCLR = SLAVE_RUN;     
    ICSP_TRIS_nMCLR = OUTPUT_PIN;
    LED_Off(RED_LED);
    LED_On(GREEN_LED);
}

void sendCmd(uint8_t b)
{   
    uint8_t i;
    ICSP_TRIS_DAT = OUTPUT_PIN; 
    for( i=0; i<8; i++){
        if ((b & 0x80) > 0)
            ICSP_DAT = 1;     // Msb first
        else
            ICSP_DAT = 0;     // Msb first
        ICSP_CLK = 1;
        b <<= 1;
        __delay_us(1);
        ICSP_CLK = 0;
        __delay_us(1);
    }    
    __delay_us(1);
}

void sendData( uint16_t data)
{
    uint8_t i;
    uint24_t w = (uint24_t)data;
    
    w = (w <<1) & 0x7ffffe;     // add start and stop bits
    ICSP_TRIS_DAT = OUTPUT_PIN; 
    for( i=0; i < 24; i++){
        if ((w & 0x800000) > 0) // Msb first
            ICSP_DAT = 1;       
        else
            ICSP_DAT = 0;       
        ICSP_CLK = 1;
        w <<= 1;
        __delay_us(1);
        ICSP_CLK = 0;
        __delay_us(1);
    }    
}

uint8_t getByte( void)
{
    uint8_t i, b;
    ICSP_TRIS_DAT = INPUT_PIN;  
    for( i=0; i < 8; i++){
        ICSP_CLK = 1;
        b <<= 1;
        __delay_us(1);
        b |= ICSP_DAT;
        ICSP_CLK = 0;
        __delay_us(1);
    }    
    return b;
}

uint16_t getData( void)
{
    uint8_t i;
    uint16_t w = 0;
    ICSP_TRIS_DAT = INPUT_PIN;  
    for( i=0; i < 24; i++){
        ICSP_CLK = 1;
        w <<= 1;
        __delay_us(1);
        w |= ICSP_DAT;
        ICSP_CLK = 0;
        __delay_us(1);
    }    
    return (w >> 1);
}

void LVP_enter( void)
{
    LED_On(RED_LED);
    LED_Off(GREEN_LED);

    ICSP_Init();                 // configure I/Os   
    ICSP_nMCLR = SLAVE_RESET;    // MCLR = Vil (GND)
    __delay_ms(10);
    sendCmd( 'M');
    sendCmd( 'C');
    sendCmd( 'H');
    sendCmd( 'P');
    __delay_ms( 5);
}

void LVP_exit( void)
{
    ICSP_Release();             // release ICSP-DAT and ICSP-CLK
}

bool LVP_inProgress( void)
{
    return (ICSP_nMCLR==SLAVE_RESET);
}

void LVP_bulkErase( void)
{
    sendCmd( CMD_LOAD_ADDRESS);  // enter config area to erase config words too
    sendData( 0x8000);
    sendCmd( CMD_BULK_ERASE);
    __delay_ms( 6);
}

void LVP_skip(uint16_t count)
{
    while(count-- > 0){
        sendCmd( CMD_INC_ADDR);     // increment address     
    }
}

void LVP_addressLoad( uint16_t address)
{
    sendCmd( CMD_LOAD_ADDRESS);  
    sendData( address);    
}

void LVP_rowWrite( uint16_t *buffer, uint8_t w)
{   
    for(; w>1; w--)     // load n-1 latches 
    {
        sendCmd( CMD_LATCH_DATA_IA);
        sendData( *buffer++);
    }
    sendCmd( CMD_LATCH_DATA);   // load last latch (n-1)
    sendData( *buffer++);
    sendCmd( CMD_BEGIN_PROG);   
    __delay_ms( 3);
    sendCmd( CMD_INC_ADDR);     // increment address only after prog. command!
}

void LVP_cfgWrite( uint16_t *cfg, uint8_t count)
{
    sendCmd( CMD_LOAD_ADDRESS); 
    sendData(0x8007 );
    while( count-- > 0){
        sendCmd( CMD_LATCH_DATA);
        sendData( *cfg++);
        sendCmd( CMD_BEGIN_PROG);
        __delay_ms( 3);
        sendCmd( CMD_INC_ADDR);    
    }
    sendCmd( CMD_LOAD_ADDRESS);   // enter code area 
    sendData(0x0000);
}
