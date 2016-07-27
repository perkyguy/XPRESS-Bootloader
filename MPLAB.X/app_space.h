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

#define CFG_ADDRESS         0x8000   
#define CFG_NUM             14
#define APP_ROW_SIZE        32

/*
This SHOULD mimic the values set in system.c, i.e.
#pragma config PLLSEL   = PLL3X     // PLL Selection (3x clock multiplier)
#pragma config CFGPLLEN = OFF       // PLL Enable Configuration bit (PLL Disabled (firmware controlled))
#pragma config CPUDIV   = NOCLKDIV  // CPU System Clock Postscaler (CPU uses system clock (no divide))
#pragma config LS48MHZ  = SYS48X8   // Low Speed USB mode with 48 MHz system clock (System clock at 48 MHz, USB clock divider is set to 8)
#pragma config FOSC     = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config PCLKEN   = OFF       // Primary Oscillator Shutdown (Primary oscillator shutdown firmware controlled)
#pragma config FCMEN    = OFF       // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO     = OFF       // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)
#pragma config nPWRTEN  = OFF       // Power-up Timer Enable (Power up timer disabled)
#pragma config BOREN    = SBORDIS   // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
#pragma config BORV     = 190       // Brown-out Reset Voltage (BOR set to 1.9V nominal)
#pragma config nLPBOR   = ON        // Low-Power Brown-out Reset (Low-Power Brown-out Reset enabled)
#pragma config WDTEN    = SWON      // Watchdog Timer Enable bits (WDT controlled by firmware (SWDTEN enabled))
#pragma config WDTPS    = 32768     // Watchdog Timer Postscaler (1:32768)
#pragma config CCP2MX   = RC1       // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN   = OFF       // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config T3CMX    = RC0       // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
#pragma config SDOMX    = RC7       // SDO Output MUX bit (SDO function is on RC7)
#pragma config MCLRE    = ON        // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)
#pragma config STVREN   = ON        // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
#pragma config LVP      = OFF       // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)
#pragma config XINST    = OFF       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)
#pragma config CP0      = OFF       // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1      = OFF       // Block 1 Code Protect (Block 1 is not code-protected)
#pragma config CP2      = OFF       // Block 2 Code Protect (Block 2 is not code-protected)
#pragma config CP3      = OFF       // Block 3 Code Protect (Block 3 is not code-protected)
#pragma config CPB      = OFF       // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD      = OFF       // Data EEPROM Code Protect (Data EEPROM is not code-protected)
#pragma config WRT0     = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1     = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
#pragma config WRT2     = OFF       // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
#pragma config WRT3     = OFF       // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)
#pragma config WRTC     = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB     = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD     = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)
#pragma config EBTR0    = OFF       // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1    = OFF       // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
#pragma config EBTR2    = OFF       // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
#pragma config EBTR3    = OFF       // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)
#pragma config EBTRB    = OFF       // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks) 
*/
const uint8_t BL_DEFAULT_CONFIGS[] = {
	0x21, // CONFIG1_L
	0x08, // CONFIG1_H
	0x1F, // CONFIG2_L
	0x3E, // CONFIG2_H
	0xFF, // CONFIG3_L
	0x91, // CONFIG3_H
	0x81, // CONFIG4_L
	0xFF, // CONFIG4_H
	0x0F, // CONFIG5_L
	0xC0, // CONFIG5_H
	0x0F, // CONFIG6_L
	0xE0, // CONFIG6_H
	0x0F, // CONFIG7_L
	0x40, // CONFIG7_H
};
// Define which config bits we will force the app to keep 0
const uint8_t APP_CONFIG_MASK[] = {
	0xFF, // CONFIG1_L
	0xFF, // CONFIG1_H
	0xFF, // CONFIG2_L
	0xFF, // CONFIG2_H
	0xFF, // CONFIG3_L
	0xFF, // CONFIG3_H
	0xFF, // CONFIG4_L
	0xFF, // CONFIG4_H
	0xFF, // CONFIG5_L
	0xFF, // CONFIG5_H
	0xFF, // CONFIG6_L
	0xFF, // CONFIG6_H
	0xFF, // CONFIG7_L
	0xFF, // CONFIG7_H
};
// Define which config bits we will force the app to keep 1
const uint8_t APP_CONFIG_SET[] = {
	0x00, // CONFIG1_L
	0xC0, // CONFIG1_H
	0x00, // CONFIG2_L
	0x00, // CONFIG2_H
	0x00, // CONFIG3_L
	0x00, // CONFIG3_H
	0x00, // CONFIG4_L
	0x00, // CONFIG4_H
	0x00, // CONFIG5_L
	0x00, // CONFIG5_H
	0x00, // CONFIG6_L
	0x00, // CONFIG6_H
	0x00, // CONFIG7_L
	0x00, // CONFIG7_H
};

/*
 Common access-point methods
 */
void APP_enter( void);
void APP_exit( void);
void APP_writeByte(uint32_t address, uint16_t data);
void APP_write(uint32_t address, uint8_t *buffer, uint8_t count);
void APP_setBootloaderConfigs(void);
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
void APP_cfgStore( uint8_t *buffer);
void APP_cfgWrite( void);

void APP_Run(void) @0x3000;
void APP_ISR_High(void) @0x3008;
void APP_ISR_Low(void) @0x3018;

#endif	/* APP_SPACE_H */
