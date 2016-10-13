/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

 Low Voltage Programming Interface 
 
  Bit-Banged implementation of the PIC16F1 (200K) LVP protocol
  Based on the PIC16F171X specification
  
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
#include "delay.h"

#define  CMD_LOAD_CONFIG      0x00
#define  CMD_LATCH_DATA       0x02
#define  CMD_READ_DATA        0x04
#define  CMD_RES_ADDRESS      0x16
#define  CMD_INC_ADDR         0x06
#define  CMD_BEGIN_PROG       0x08
#define  CMD_BULK_ERASE       0x09

#define CFG_ADDRESS 0x8000   // for all pic16f1xxx

uint16_t lvp_address = 0;

void LVP_Init(void )
{
    LVP_D_TRIS = INPUT;
    LVP_C_LAT = 0;
    LVP_C_TRIS = OUTPUT;
    LVP_M_LAT = SLAVE_RUN;
    LVP_M_TRIS = OUTPUT;
}

void LVP_Release(void)
{
    LVP_D_TRIS  = INPUT;
    LVP_C_TRIS  = INPUT;
    LVP_M_LAT = SLAVE_RUN;     
    LVP_M_TRIS  = OUTPUT;
}

void sendCmd(uint8_t b)
{   
    uint8_t i;
    LVP_D_TRIS = OUTPUT; 
    for(i=0; i<6; i++){      // 6-bit commands
        if ((b & 1) > 0)     // Lsb first
            LVP_D_LAT = 1;     
        else
            LVP_D_LAT = 0;     
        LVP_C_LAT = 1;
        b >>= 1;            // shift right
        __delay_us(1);
        LVP_C_LAT = 0;
        __delay_us(1);
    }    
    __delay_us(1);
}

void sendWord(uint16_t w)
{
    uint8_t i;
    
    LVP_D_TRIS = OUTPUT; 
    for(i=0; i < 16; i++){
        if ((w & 1) > 0)        // Lsb first
            LVP_D_LAT = 1;       
        else
            LVP_D_LAT = 0;       
        LVP_C_LAT = 1;
        w >>= 1;                // shift right
        __delay_us(1);
        LVP_C_LAT = 0;
        __delay_us(1);
    }    
}

void sendData(uint16_t w)
{
    w = (w <<1) & 0x7ffe;     // add start and stop bits
    sendWord(w);
}

uint16_t getData(void)
{
    uint8_t i;
    uint16_t w = 0;
    LVP_D_TRIS = INPUT;  
    for(i=0; i < 16; i++){  // 16-bit word
        LVP_C_LAT = 1;
        w >>= 1;            // shift right
        __delay_us(1);
        w |= (LVP_D_PORT) ? 0x8000 : 0;  // Lsb first     
        LVP_C_LAT = 0;
        __delay_us(1);
    }    
    return (w >> 1) & 0x3fff;    
}

void LVP_enter(void)
{
    LVP_Init();                 // configure I/Os   
    LVP_M_LAT = SLAVE_RESET;    // MCLR = Vil (GND)
    delay(10);
    sendWord(0x4850);           // "MCHP" Lsb first
    sendWord(0x4D43);
    LVP_C_LAT = 1;
    __delay_us(1);
    LVP_C_LAT = 0;
    __delay_us(1);
    delay(5);
    lvp_address = 0;
}

void LVP_exit(void)
{
    LVP_Release();             // release ICSP-DAT and ICSP-CLK
}

bool LVP_inProgress(void)
{
    return (LVP_M_LAT == SLAVE_RESET);
}

void LVP_bulkErase(void)
{
    sendCmd(CMD_LOAD_CONFIG);  // enter config area to erase ID words too
    sendData(CFG_ADDRESS);
    lvp_address = CFG_ADDRESS;
    sendCmd(CMD_BULK_ERASE);
    delay(6);
}

void skip(uint16_t count)
{
    while(count-- > 0){
        sendCmd(CMD_INC_ADDR);     // increment address 
        lvp_address++;
    }
}

void LVP_addressLoad(uint16_t address)
{
    if (address >= CFG_ADDRESS) {
        sendCmd(CMD_LOAD_CONFIG);
        sendData(CFG_ADDRESS);
        lvp_address = CFG_ADDRESS;
    }
    if (address < lvp_address){
        sendCmd(CMD_RES_ADDRESS);
        lvp_address = 0;
    }
    skip(address - lvp_address);
}

void LVP_rowWrite(uint16_t *buffer, uint8_t w)
{   
    for(; w>1; w--)     // load n-1 latches 
    {
        sendCmd(CMD_LATCH_DATA);
        sendData(*buffer++);
        sendCmd(CMD_INC_ADDR); 
        lvp_address++;
    }
    sendCmd(CMD_LATCH_DATA);   // load last latch (n-1)
    sendData(*buffer++);
    sendCmd(CMD_BEGIN_PROG);   
    delay(3);
    sendCmd(CMD_INC_ADDR);     // increment address only after prog. command!
    lvp_address++;
}

void LVP_cfgWrite(uint16_t *cfg, uint8_t count)
{
    LVP_addressLoad(0x8007);
    while(count-- > 0){
        sendCmd(CMD_LATCH_DATA);
        sendData(*cfg++);
        sendCmd(CMD_BEGIN_PROG);
        delay(3);
        sendCmd(CMD_INC_ADDR);    
        lvp_address++;
    }
    sendCmd(CMD_RES_ADDRESS);   // enter code area 
    lvp_address = 0;
}

uint16_t LVP_read(void)
{
    
    sendCmd(CMD_READ_DATA);
    uint16_t data = getData();
    sendCmd(CMD_INC_ADDR);
    return data;
}
