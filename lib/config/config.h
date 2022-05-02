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
#include "crsf_protocol.h"

uint8_t device_idx;   // current device index
crsfPayloadLinkstatistics_s LinkStatistics; // Link Statisitics Stored as Struct
char *recv_param_ptr;

uint8_t count_params_loaded();

extern crsf_device_t crsf_devices[];

extern elrs_info_t local_info;

extern elrs_info_t elrs_info;

typedef enum {
    MODULE_UNKNOWN,
    MODULE_ELRS,
    MODULE_OTHER,
} module_type_t;

uint8_t protocol_module_is_elrs();
extern module_type_t module_type;
#define MODULE_IS_ELRS     (module_type == MODULE_ELRS)
#define MODULE_IS_UNKNOWN  (module_type == MODULE_UNKNOWN)

int Aileron_value = 0;        // values read from the pot 
int Elevator_value = 0; 
int Throttle_value=0;
int Rudder_value = 0; 

int Arm = 0;        // switch values read from the digital pin
int FlightMode = 0; 

 // Define RC input Offset
int Aileron_OFFSET = 0;        // values read from the pot 
int Elevator_OFFSET  = 0; 
int Throttle_OFFSET =0;
int Rudder_OFFSET  = 0; 

extern uint8_t next_param;   // parameter and chunk currently being read

void sync_crsf(int32_t add);
void serialEvent();



//NUM_TRIM_ELEMS + NUM_BOX_ELEMS + NUM_BAR_ELEMS + NUM_TOGGLE_ELEMS
#ifndef NUM_ELEMS
    #define NUM_ELEMS (6 + 8 + 8 + 4 + 1)
#endif



//button bouncer
int testButtonPressed=0;
extern bool powerChangeHasRun;


#define TEMPSTRINGLENGTH 400 //This is the max dialog size (80 characters * 5 lines)
                             //We could reduce this to ~240 on the 128x64 screens
                             //But only after all sprintf are replaced with snprintf
                             //Maybe move this to target_defs.h
extern char tempstring[TEMPSTRINGLENGTH];

static const char *hdr_str_cb(const void *data);


static void crsfdevice_init();
void bt_handle(uint8_t value);

static uint32_t crsfTime;

class Config {

public:
    //elrs timing
    // for calculate main loop time to sync with elrs tx module
    uint32_t lastCrsfTime;
    uint32_t updateInterval=0;
    int32_t correction;

    uint8_t params_loaded;     // if not zero, number received so far for current device

    void qsd();
};


extern Config config;
