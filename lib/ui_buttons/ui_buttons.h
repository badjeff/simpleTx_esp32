#include "Arduino.h"

  #ifndef UI_BUTTONS_H
      #define UI_BUTTONS_H
      
///const int upBt = 12;
//const int downBt = 4;
const int enterBt = 0;
const int backBt = 5;

//setup menus
static int selected = 0;
static int subSelected = -1;
static int mmOptionSelected = -1;
static int entered = -1; //-2 idle // -1 main menu // 0 options/submenu
static bool menu_loaded = false;



void read_ui_buttons (uint8_t next_param,uint8_t next_chunk);


      #endif /* EXAMPLE_H */