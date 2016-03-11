/* 
 * File:   app_space.h
 */
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef APP_SPACE_H
#define	APP_SPACE_H


#define APP_init() APP_exit();
#define APP_END_ADDRESS     END_FLASH
#define APP_START_ADDRESS   0x2FFF

//#define APP_INT_LP_VECTOR   0x3008
//#define APP_INT_HP_VECTOR   0x3018
//#define APP_CONFIG_STORE    0x3FF0

#define CFG_ADDRESS         0x8000   
#define CFG_NUM             5
#define APP_ROW_SIZE        32

//// Testing
//#define ICSP_TRIS_nMCLR     TRISBbits.TRISB4
//#define ICSP_nMCLR          LATBbits.LATB4
//#define SLAVE_RUN           0
//#define SLAVE_RESET         1
//#define INPUT_PIN           1
//#define OUTPUT_PIN          0
// End test


/*
 Common access-point methods
 */
void APP_enter( void);
void APP_exit( void);
void APP_writeByte(uint32_t address, uint16_t data);
void APP_write(uint32_t address, uint8_t *buffer, uint8_t count);

/* 
 Status methods
 */
bool APP_inProgress(void);
/*
 Lower level methods
 */
bool APP_addressInRange(uint32_t address);
void APP_bulkErase( void);
void APP_rowWrite( uint32_t address, uint8_t *buffer, uint8_t count);
void APP_rowErase( uint32_t address);
void APP_cfgWrite( uint8_t *buffer);

void APP_Run(void) @0x3000;
void APP_ISR_High(void) @0x3008;
void APP_ISR_Low(void) @0x3018;

#endif	/* APP_SPACE_H */


