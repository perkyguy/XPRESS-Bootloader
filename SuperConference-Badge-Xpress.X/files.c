/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

 Memory Disk Drive File System Emulator
    64-byte bulk-packet optimized

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
 
#include "files.h"
#include "string.h"


//------------------------------------------------------------------------------
//Master boot record (MBR) at LBA = 0
//------------------------------------------------------------------------------
void MasterBootRecordGet( uint8_t * buffer, uint8_t seg) 
{  // fabricate an MBR structure in the RAM buffer segment
    memset( buffer, 0, MSD_OUT_EP_SIZE);  // clear buffer  
    if (seg < 6)   return; // segments 0-5 from 0x000 - 0x17f are empty
//Code Area
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//0x0000
// ...
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                     //0x0180
// IBM 9 byte/entry x 4 entries primary partition table
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                             //0x018A
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x0190
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //0x01A0
//
// 0x00, 0x00,                                                                                     //0x01AE
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                 //0x01B0

    if (seg < 7)    {   // segment 6: 0x180 - 0x1bf 
        // Disk signature             //0x01B8
        buffer[ 0x1b8-0x180] = 0xF5; buffer[ 0x1b9-0x180] = 0x8B; 
        buffer[ 0x1ba-0x180] = 0x16; buffer[ 0x1bb-0x180] = 0xEA;                                                                 

    // Table of Primary Partitions (16 bytes/entry x 4 entries)
    // Note: Multi-byte fields are in little endian format.
    // Partition Entry 1                                                                             //0x01BE
        buffer[ 0x1be-0x180] = 0x00;                  // Status - 0x80 (bootable), 0x00 (not bootable), other (error)
        buffer[ 0x1bf-0x180] = 0x01;                  // Cylinder
    }
    else { // segment 7: 0x1c0 - 0x1ff 
        buffer[ 0x1c0-0x1c0] = 0x01;                  // Head
        buffer[ 0x1c1-0x1c0] = 0x00;                  // Sector address of first sector in partition
        buffer[ 0x1c2-0x1c0] = 0x01;                  // Partition type - 0x01 = FAT12 up to 2MB -0xE = FAT16
        buffer[ 0x1c3-0x1c0] = 0x07;                  // Cylinder
        buffer[ 0x1c4-0x1c0] = 0xFF;                  // Head
        buffer[ 0x1c5-0x1c0] = 0xE6;                  // Sector address of last sector in partition
        buffer[ 0x1c6-0x1c0] = 0x01;                  // Logical Block Address (LBA) of first sector in partition
        // 00. 00. 00
        buffer[ 0x1ca-0x1c0] = (uint8_t) DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE;
        buffer[ 0x1cb-0x1c0] = (uint8_t)(DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE >>  8); 
        buffer[ 0x1cc-0x1c0] = (uint8_t)(DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE >> 16); 
        buffer[ 0x1cd-0x1c0] = (uint8_t)(DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE >> 24); // Length of partition in sectors 
        // Note: (MBR sits at LBA = 0, and is not in the partition.)
    //Partition Entry 2
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x01CE
    //Partition Entry 3
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x01DE
    //Partition Entry 4
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x01EE
    //MBR signature         //0x01FE
        buffer[ 0x1fe-0x1c0] = 0x55; 
        buffer[ 0x1ff-0x1c0] = 0xAA;
    }
}
//------------------------------------------------------------------------------
// Partition BOOT sector at LBA = 1
//------------------------------------------------------------------------------
// Physical Sector - 1, Logical Sector - 0.  
// This is the first sector in the partition, known as the VBR, "volume boot record" 
void VolumeBootRecordGet( uint8_t * buffer, uint8_t seg) 
{  // fabricate an MBR structure in the RAM buffer
    memset( buffer, 0, MSD_OUT_EP_SIZE);              // clear buffer  
    if (seg == 0) { // segment from 000 to 0x03f
    buffer[ 0x000]=0xEB;                                // (legacy) Jump instruction
    buffer[ 0x001]=0x3C; 
    buffer[ 0x002]=0x90;                                
    memcpy( (void*)&buffer[3], (void*)"MSDOS5.0", 8);   // OEM Name "MSDOS5.0"
    buffer[ 0x00b] = (FILEIO_CONFIG_MEDIA_SECTOR_SIZE & 0xFF);        // Bytes per sector 
    buffer[ 0x00c] = (FILEIO_CONFIG_MEDIA_SECTOR_SIZE>>8);            
    buffer[ 0x00d] = 0x01;                              // Sectors per cluster
    buffer[ 0x00e] = 0x01;  // Reserved sector count (1 for FAT12 or FAT16)
    // 0x00,			
    buffer[ 0x010] = 0x01;                              // number of FATs 
    buffer[ 0x011] = DRV_FILEIO_CONFIG_INTERNAL_FLASH_MAX_NUM_FILES_IN_ROOT;
    // 0x00,		// Max number of root directory entries - 16 files allowed
    // 0x00, 0x00,  // total sectors (0x0000 means: use the 4 byte field at offset 0x20 instead)
    buffer[ 0x015] = 0xF8;			    //Media Descriptor
    buffer[ 0x016] = DRV_FILEIO_INTERNAL_FLASH_NUM_FAT_SECTORS;
    // 0x00,                                            // Sectors per FAT
    buffer[ 0x018] = 0x3F; 
    // 0x00,                                            // Sectors per track
    buffer[ 0x01A] = 0xFF; 
    // 0x00,                                            // Number of heads
    buffer[ 0x01c] = 0x01; // 0x00, 0x00, 0x00,         // Hidden sectors (previous partition)
    buffer[ 0x020] = (uint8_t)  DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE; 
    buffer[ 0x021] = (uint8_t)( DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE >> 8); 
    buffer[ 0x022] = (uint8_t)( DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE >> 16); 
    buffer[ 0x023] = (uint8_t)( DRV_FILEIO_INTERNAL_FLASH_PARTITION_SIZE >> 24);	//Total sectors (when uint16_t value at offset 20 is 0x0000)

    buffer[ 0x024] = 0x00;			// Physical drive number
    buffer[ 0x025] = 0x00;			// Reserved("current head")
    buffer[ 0x026] = 0x29;			// Signature
    buffer[ 0x027] = 0x32;          // ID (serial number)
    buffer[ 0x028] = 0x67; 
    buffer[ 0x029] = 0x94; 
    buffer[ 0x02a] = 0xC4;		
    memcpy( (void*)&buffer[ 0x02b], (void*)"HackABadge ", 11); // Volume Label (11 bytes)
    memcpy( (void*)&buffer[ 0x036], (void*)"FAT12   ", 8);     // FAT system ( 8 bytes)
    return;
    }
    if ( seg < 7) return;  // segments 1-6 from 0x040 to 0x1c0 are empty
//Operating system boot code
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// ...
    else {
        buffer[ 0x1fe-0x1c0] = 0x55; 
        buffer[ 0x1ff-0x1c0] = 0xAA;			// signature End of sector (0x55AA)
    }
}

//------------------------------------------------------------------------------
// First FAT sector at LBA = 2
// Note: This table consists of a series of 12-bit entries, and are fully packed 
// (no pad bits).  This means every other byte is a "shared" byte, that is split
// down the middle and is part of two adjacent 12-bit entries.  
// The entries are in little endian format.

void FATRecordInit( void)
{
}

void FATRecordGet( uint8_t * buffer, uint8_t seg)
{
    memset( (void*)buffer, 0, MSD_IN_EP_SIZE);
    if (seg == 0) {
        buffer[ 0] = 0xF8;      // Copy of the media descriptor 0xFF8
        buffer[ 1] = 0xFF;   
        buffer[ 2] = 0xFF;
        buffer[ 3] = 0xFF;      // 2 - first/last cluster in short file chain
        buffer[ 4] = 0x0F;      // readme.htm
    }
}

void FATRecordSet( uint8_t * buffer, uint8_t seg)
{   
}

//------------------------------------------------------------------------------
// ROOT sector at LBA = 3

const char readme[] = "<meta http-equiv=\"refresh\" content=\"0;URL='http://mplabxpress.microchip.com'\" />";

uint8_t readme_size( void) {
    return sizeof( readme);     // required by direct.c (external references return always size 0!?)
}

const uint8_t entry0[ ROOT_ENTRY_SIZE] = { 
    'H','a','c','k','A','B','a','d','g','e',' ',  // Drive Name (11 characters, padded with spaces)
    0x08, //specify this entry as a volume label
    0x00, //Reserved

    0x00,           // Creation time, fine res 10 ms units (0-199)
    TIMEL(MAJOR, MINOR, 0),     // Creation time, hour/min/sec
    TIMEH(MAJOR, MINOR, 0),     // Creation time, hour/min/sec
    DATEL(YEAR, MONTH, DAY),    // Creation date, YMD 
    DATEH(YEAR, MONTH, DAY),    // Creation date, YMD
    
    DATEL(YEAR, MONTH, DAY),    // Last Access date, YMD
    DATEH(YEAR, MONTH, DAY),    // Last Access date, YMD
    0x00, 0x00,     // Extended Attributes
    
    TIMEL(MAJOR, MINOR, 0),     // Last Modified time h/m/s
    TIMEH(MAJOR, MINOR, 0),     // Last Modified time h/m/s
    DATEL(YEAR, MONTH, DAY),    // Last Modified date, YMD
    DATEH(YEAR, MONTH, DAY),    // Last Modified date, YMD

    0x00, 0x00,                 // First FAT cluster (none)
    0x00, 0x00, 0x00, 0x00,     // File Size (number of bytes)
 };


 const  uint8_t entry1[ ROOT_ENTRY_SIZE] = {
    'R','E','A','D','M','E',' ',' ',    // File name (exactly 8 characters)
    'H','T','M',                        // File extension (exactly 3 characters)
    0x20,           // specify this entry as a regular file
    0x00,           // Reserved
    0x00,           // Creation time, fine res 10 ms units (0-199)
    TIMEL(MAJOR, MINOR, 0),     // Creation time, hour/min/sec
    TIMEH(MAJOR, MINOR, 0),     // Creation time, hour/min/sec
    DATEL(YEAR, MONTH, DAY),    // Creation date, YMD 
    DATEH(YEAR, MONTH, DAY),    // Creation date, YMD
    
    DATEL(YEAR, MONTH, DAY),    // Last Access date, YMD
    DATEH(YEAR, MONTH, DAY),    // Last Access date, YMD
    0x00, 0x00,     // Extended Attributes
    
    TIMEL(MAJOR, MINOR, 0),     // Last Modified time h/m/s
    TIMEH(MAJOR, MINOR, 0),     // Last Modified time h/m/s
    DATEL(YEAR, MONTH, DAY),    // Last Modified date, YMD
    DATEH(YEAR, MONTH, DAY),    // Last Modified date, YMD
    
    0x02, 0x00,     // First FAT cluster (#2 is the first available)
    sizeof(readme), 0x00, 0x00, 0x00,         // README string size (<256)
};

void RootRecordInit( void)
{
}

void RootRecordGet( uint8_t * buffer, uint8_t seg)
{
    memset( (void*)buffer, 0, FILEIO_CONFIG_MEDIA_SECTOR_SIZE);
   if (seg == 0) {
        memcpy( (void*)&buffer[ 0], (const void*)entry0, ROOT_ENTRY_SIZE ); 
        // add the README.HTM file
        memcpy( (void*)&buffer[ ROOT_ENTRY_SIZE], (const void*)entry1, ROOT_ENTRY_SIZE );
    }
}

void RootRecordSet( uint8_t *buffer, uint8_t seg)
{
}
