Hack-A-Day Superconference 2016 Badge BootLoader
=============

USB-MSD Bootloader
------------------

This application is designed to act as a boot loader for the 18F25K50. Specifically it was meant to be 
used for the Hackaday Superconference 2016 Badge. It is based on the same philosophy of the MPLab Xpress Evaluation Board's programmer.

Additional information about the Badge can be found at
[https://hackaday.io/project/9509-badge-for-hackaday-belgrade-conference](<https://hackaday.io/project/9509-badge-for-hackaday-belgrade-conference>)

Additional information about the MPLAB Xpress project can be found at
[https://mplabxpress.microchip.com](<https://mplabxpress.microchip.com>)

License
-------

This application is licensed under the Apache v2 license (see "LICENSE" file in
root directory of the repository). To request to license this code under the MLA
license (www.microchip.com/mla_license), please contact
mla_licensing@microchip.com.


Usage
-----
-   Attach a micro-USB cable from your PC to the USB port on the badge

-   Press and hold the RESET and Power buttons

-   Release the RESET button

-   Release the Power button

-   A new mass storage device named HackABadge should appear and an LED should be blinking on the badge

-   Copy a hex file (appropriately addressed) to this device

-   Press the Power button again to exit the bootloader

-   Upon exiting the bootloader, or on a restart, code execution will begin at **0x2B00** (Kernel's entry point)

Memory Locations
---------

| Address| Function                      |
|:------:|:------------------------------|
| 0x0000 |  Bootloader Begin             |
| ...    |                               |
| 0x2AFF |  Bootloader End               |
| 0x2B00 |  Kernel Begin                 |
| 0x2B08 |  Kernel Hi Priority Interrupt |
| 0x2B18 |  Kernel Lo Priority Interrupt |
| ...    |                               |
| 0x32FF |  Kernel End                   |
| 0x3300 |  App Start                    |
| 0x3308 |  App Hi Priority Interrupt    |
| 0x3318 |  App Lo Priority Interrupt    |
| ...    |                               |
| 0x7FFF |  Application End              |

-   The bootloader currently reserves **0x0000-0x2AFF**

-   The badge's kernel is assumed to be in **0x2B00-0x32FF**

-   Application code will only be written if it is at or above **0x3300**. Any code in HEX file below this value will **not** be written.
    
-   Application interrupt vectors are moved to **0x3308** and **0x3318**

Limitations
---------------

-   Only will modify program flash memory. DataEE and Configuration word changes are **not** currently supported


Product Support
---------------

-   This application runs on the PIC18LF25K50.

-   The input (file) parsing algorithm is compatible with all PIC18 INTEL
    Hex files produced by the MPLAB XC8 compiler.


Folder Structure
----------------

-   *MPLAB.X* - contains the main application source files

-   *framework* - elements of the MLA - USB and File System open source
    libraries (note: the MSD portion has been customised to reduce considerably
    RAM usage)

-   *utilities* - contains the Windows signed drivers for the Virtual COM port
    (OS X and Linux users do not need it)

-   *bsp* - board support package (currently only the XPRESS evaluation board)

 
