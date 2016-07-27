
/*
 * File: lvp.c
 *
 * Bit Banged implementation of the PIC16F1 (250K) LVP protocol
 * First draft based on the PIC16F188XX specification
 *      - Additional timing details have been inspired by PICKIT3
 *
 */
#include "app_space.h"
#include "memory.h"
#include "leds.h"
#include <string.h>
static bool APP_PROGRAMMING;
static uint8_t APP_CONFIGS[CFG_NUM];

void APP_enter( void) {
    APP_PROGRAMMING = true;
}
void APP_exit( void) {
	// If we were programming, then we're done. Otherwise, we failed...?
	APP_PROGRAMMING = false;
}
void APP_writeByte(uint32_t address, uint16_t data) {
	if (!APP_inProgress()) {
		// We got here before things got initialized...do that!
		//TODO: set programming flag in enter(), and any other BL flags needed
		APP_enter();
		APP_bulkErase();
	}
	if (!APP_addressInRange(address)) {return;}
	uint8_t flashRdBufPtr[APP_ROW_SIZE<<1];
	FLASH_WriteByte(address, flashRdBufPtr, data);
}
void APP_write(uint32_t address, uint8_t *buffer, uint8_t count) {
	// first check if we are in progress already
	if (!APP_inProgress()) {
		// We got here before things got initialized...do that!
		//TODO: set programming flag in enter(), and any other BL flags needed
		APP_enter();
		APP_bulkErase();
	}
	if (address >= CFG_ADDRESS) {
		// Store the configuration words
		APP_cfgStore( buffer );
	}
	else { 
		// normal row programming sequence
		APP_rowWrite( address, buffer, count);
	}
}

bool APP_inProgress( void) {
	// If we're erasing or programming we're busy
	return APP_PROGRAMMING;
}

bool APP_addressInRange(uint32_t address) {
	return address > APP_START_ADDRESS && address < APP_END_ADDRESS;
}
void APP_bulkErase( void) {
    // Erase the APP Space
	uint32_t curr_address;
	// Make sure we are showing that we are doing something...
	APP_PROGRAMMING = true;
	for (curr_address = APP_START_ADDRESS; curr_address < APP_END_ADDRESS; curr_address += APP_ROW_SIZE) {
		APP_rowErase(curr_address);
	}
}
void APP_rowWrite( uint32_t address, uint8_t *buffer, uint8_t count){
	if (!APP_addressInRange(address)) {return;}
	// Only write if we are in the right spot
	uint8_t flashRdBufPtr[APP_ROW_SIZE<<1];
	FLASH_WriteSome(address, flashRdBufPtr, buffer, count);
}


void APP_rowErase( uint32_t address) {
	if (!APP_addressInRange(address)) {return;}
	// Only write if we are in the right spot
	FLASH_EraseBlock(address);
}

void APP_cfgStore( uint8_t *cfg) {
    uint8_t i;
    
    for(i = 0; i < CFG_NUM; i++){
    	APP_CONFIGS[i] = cfg[i];
//    	APP_CONFIGS[i] &= APP_CONFIG_MASK[i];
//    	APP_CONFIGS[i] |= APP_CONFIG_SET[i];
    }
}

void APP_cfgWrite( void){
    FLASH_WriteConfigs(APP_CONFIGS);
//    uint8_t wrCfgData[] = {
//        0x21,0x28,0x59,0x31,0xD3,0x81,0x0F,0xc0,0x0F,0xE0,0x0F,0x40
//    };
//	uint8_t wrBuf[14]; 
//    memset(wrBuf, 0xff, sizeof(wrBuf)); 
//    memcpy(wrBuf, wrCfgData, 14);
//    FLASH_WriteConfigs(wrCfgData);
    LED_On(RED_LED);
}     

void APP_setBootloaderConfigs(void){
	uint8_t i;
	// Let's save what we have currently, so we can bring it back later
//	for(i = 0; i < CFG_NUM; i++){
//		APP_CONFIGS[i] = FLASH_ReadByte((uint32_t)(0x300000+i));
//	}
	FLASH_WriteConfigs(BL_DEFAULT_CONFIGS);
}