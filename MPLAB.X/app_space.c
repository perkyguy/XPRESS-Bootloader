
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

static bool APP_PROGRAMMING;

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
	APP_PROGRAMMING = true;
	if (!APP_addressInRange(address)) {return;}
	uint8_t flashRdBufPtr[APP_ROW_SIZE<<1];
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
	if (!APP_addressInRange(address)) {return;}
	APP_PROGRAMMING = true;
	if (address >= CFG_ADDRESS) {    // use the special config word function
		APP_cfgWrite( buffer );
	}
	else { // normal row programming sequence
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
	for (curr_address = APP_START_ADDRESS; curr_address < APP_END_ADDRESS; curr_address += APP_ROW_SIZE) {
		APP_rowErase(curr_address);
	}
	// Make sure we are showing that we are doing something...
	APP_PROGRAMMING = true;
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

void APP_cfgWrite( uint8_t *cfg) {

}
