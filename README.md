XPRESS-BootLoader
=============

USB-MSD Bootloader
------------------

This application is designed to act as a boot loader for the MPLAB
Xpress Evaluation Board's 18F25K50. Specifically it was meant to be 
used for the Hackaday Belgrade Badge.

Additional information about the Hackaday Belgrade Badge can be found at
[https://hackaday.io/project/9509-badge-for-hackaday-belgrade-conference](<https://hackaday.io/project/9509-badge-for-hackaday-belgrade-conference>)

Additional information about the MPLAB Xpress project can be found at
[https://mplabxpress.microchip.com](<https://mplabxpress.microchip.com>)

License
-------

This application is licensed under the Apache v2 license (see "LICENSE" file in
root directory of the repository). To request to license this code under the MLA
license (www.microchip.com/mla\_license), please contact
mla\_licensing\@microchip.com.


Usage
-----
-   Power off the board and press Button S1 (RB0 to digital low), Xpress Board has this labeled as "Reset"

-   Apply power to the board, and then release the button (RB0 to Digital High)

-   A new mass storage device named HackABadge should appear

-   Copy a hex file (appropriately addressed) to this device

-   Press the button S1 again to exit the bootloader

-   Upon exiting the bootloader, or on a restart, code execution will begin at **0x3000**

Specifics/Limitations
---------
-   The bootloader currently reserves 0x0000-0x2FFF of the Program Flash. Application code **must** stay above this. Any code in HEX file below this value will **not** be written.
    
-   Application interrupt vectors are moved to 0x3008 and 0x3018

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

 
