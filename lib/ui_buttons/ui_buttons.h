#include "Arduino.h"

  #ifndef UI_BUTTONS_H
      #define UI_BUTTONS_H
      
const int upBt = 12;
const int downBt = 4;
const int enterBt = 0;
const int backBt = 5;

//setup menus
extern int selected;
extern int subSelected;
extern int mmOptionSelected;
extern int entered; //-2 idle // -1 main menu // 0 options/submenu
extern bool menu_loaded;



void read_ui_buttons (uint8_t next_param,uint8_t next_chunk);


      #endif /* EXAMPLE_H */