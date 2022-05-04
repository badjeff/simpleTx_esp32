/*
 * This file is part of Simple TX
 *
 * Simple TX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Simple TX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 =======================================================================================================
 Simple TX CONFIG OPTIONS (comment out unneeded options)
 =======================================================================================================


 */
#pragma once

#include "crsf_protocol.h"




extern int Aileron_value;        // values read from the pot 
extern int Elevator_value; 
extern int Throttle_value;
extern int Rudder_value; 

extern int Arm;        // switch values read from the digital pin
extern int FlightMode; 

 // Define RC input Offset
extern int Aileron_OFFSET;        // values read from the pot 
extern int Elevator_OFFSET; 
extern int Throttle_OFFSET;
extern int Rudder_OFFSET; 


//NUM_TRIM_ELEMS + NUM_BOX_ELEMS + NUM_BAR_ELEMS + NUM_TOGGLE_ELEMS
#ifndef NUM_ELEMS
    #define NUM_ELEMS (6 + 8 + 8 + 4 + 1)
#endif



//button bouncer
extern int testButtonPressed;
extern bool powerChangeHasRun;


#define TEMPSTRINGLENGTH 400 //This is the max dialog size (80 characters * 5 lines)
                             //We could reduce this to ~240 on the 128x64 screens
                             //But only after all sprintf are replaced with snprintf
                             //Maybe move this to target_defs.h
extern char tempstring[TEMPSTRINGLENGTH];


static const char *hdr_str_cb(const void *data);
uint8_t count_params_loaded();


static void crsfdevice_init();
void bt_handle(uint8_t value);
