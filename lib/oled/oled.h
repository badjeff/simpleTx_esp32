#pragma once 
#include "crsf_protocol.h"
//#include <U8x8lib.h>
#include <U8g2lib.h>

//U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;

class Oled {
  public:
    static void init();
    static void setMainScreen(char *name, crsfLinkStatistics_t LinkStats,uint8_t bpkts, uint8_t gpkts);
    static void setMainMenuItems(int selected);
    static void selectOptionMainMenu(int selected, int mmOptionSelected);
    static void setSubMenuItems(int selected,int subSelected);
    static void println(char* tmp);
    static void PrintCenter(uint8_t y,char * tmp);
    static void PrintRight(char *tmp);
    static void PrintRight(uint8_t y,char *tmp);
};