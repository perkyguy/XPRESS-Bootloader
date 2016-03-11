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
#include <stdbool.h>
#include <stdint.h>

#include "io_mapping.h"

// semantic versioning
#define MAJOR   2       // change only when incompatible changes are made
#define MINOR   1       // change when adding functionality or fixing bugs

// this release date
#define YEAR            2016
#define MONTH           2       // January=1, February=2 ..
#define DAY             7       // Day:1..31

#define MAIN_RETURN void

/*********************************************************************
* Function: void SYSTEM_Initialize(void)
*
* Overview: Initializes the system.
*
********************************************************************/
void SYSTEM_Initialize(void);

/*********************************************************************
* Function: void SYSTEM_Tasks(void)
*
* Overview: Runs system level tasks that keep the system running
*
* PreCondition: System has been initalized with SYSTEM_Initialize()
*
********************************************************************/
#define SYSTEM_Tasks()

