
//menu code based on work from:
//https://www.youtube.com/watch?v=fjsmVzMdqyU
//https://drive.google.com/drive/folders/1w3jhBr031lmTeeE9Du9uZkYuoHx6m21P
//dsnmechanic@gmail.com 
//instagram: dsnmechanics

#include <Arduino.h>
#include "menus.h"
#include "crsf.h"
#include "gpio.h"
#include "oled.h"
#include "ui_buttons.h"

Menu menuItems[CRSF_MAX_PARAMS];


U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0,U8X8_PIN_NONE,22,21);

void Menu::ChangeParam(uint8_t param, uint8_t cmd){
  //db_out.printf("ChangeParam: %s\n",menuItems[selected].name);
  buildElrsPacket(crsfCmdPacket,param,cmd);
  elrsWrite(crsfCmdPacket,8,200000);

  delay(500);

  next_chunk = 0;
  CRSF_read_param(crsfCmdPacket,param,next_chunk);
  elrsWrite(crsfCmdPacket,8,20000); 
}


void Menu::loadMainMenu(char *load) {
  display.clearDisplay();
  display.drawStr(0,8,load);
  display.sendBuffer();
  delay(500);
}
