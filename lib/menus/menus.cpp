
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

static void parse_bytes(enum data_type type, char **buffer, char *dest) {
    switch (type) {
    case UINT8:
        *(uint8_t *)dest = (uint8_t) (*buffer)[0];
        *buffer += 1;
        break;
    case INT8:
        *(int8_t *)dest = (int8_t) (*buffer)[0];
        *buffer += 1;
        break;
    case UINT16:
        *(uint16_t *)dest = (uint16_t) (((*buffer)[0] << 8) | (*buffer)[1]);
        *buffer += 2;
        break;
    case INT16:
        *(int16_t *)dest = (int16_t) (((*buffer)[0] << 8) | (*buffer)[1]);
        *buffer += 2;
        break;
    case FLOAT:
        *(int32_t *)dest = (int32_t) (((*buffer)[0] << 24) | ((*buffer)[1] << 16)
                     |        ((*buffer)[2] << 8)  |  (*buffer)[3]);
        *buffer += 4;
        break;
    default:
        break;
    }
}


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

char * Menu::getMainMenuItem_StatusText() {  
  if (optionsMainMenu[status]) return optionsMainMenu[status];
  return 0;
}	             
     
void Menu::displayInfo() {			
  if (name) {
    db_out.printf("displayInfo :: %u:%s:%u:%u:%u:%u:%u\n",id,name,parent,p_type,hidden,max_value,status);
      if (max_value) {
        for (size_t i = 0; i <= max_value; i++) {
          db_out.printf("%s:",
          optionsMainMenu[i]);
        }
    }      
        
    if (p_type == 9) db_out.printf(" :: OPT");  
    if (p_type == 11) db_out.printf(" :: MainMenuItem");
    if (p_type == 12) db_out.printf(" :: INFO");
    if (p_type == 13) db_out.printf(" :: CMD");  

    db_out.printf("\n");  
  }
}

void Menu::divideValueParam (char *values) {
  char *start = (char *)values;
  uint8_t count = 0;
  for (char *p = (char *)values; *p; p++) {
    if (*p == ';') {
      int len = (strlen(start)-strlen(p));
      optionsMainMenu[count] = new char[len+1];
      strlcpy(optionsMainMenu[count],start,len+1);
  
      start = p+1;
      count += 1;
    }
  }
  int len = strlen(start);
  optionsMainMenu[count] = new char[len+1];
  strlcpy(optionsMainMenu[count],start,len+1);
        
  max_value = count;   
}

void Menu::getParams(char *buffer,int iid) {
  //db_out.printf("get P: %i\n",iid);
  id = iid;
  parent = *buffer++;
  //set main menu items
  p_type = *buffer & 0x7f;
  hidden = *buffer++ & 0x80;
  
  name = new char[strlen(buffer)+1];
  strlcpy(name, (const char *)buffer,strlen(buffer)+1);
  buffer += strlen(buffer) + 1;
  
  switch (p_type) {
  case 9: //text
    value = new char[strlen(buffer)+1];
    strlcpy(value, (const char *)buffer,strlen(buffer)+1);
    buffer += strlen(buffer) + 1;

    divideValueParam(value);
    //db_out.printf("%s",value);
    parse_bytes(UINT8, &buffer, (char *) &status);
    parse_bytes(UINT8, &buffer, (char *) &min_value);
    parse_bytes(UINT8, &buffer, (char *) &count);  // don't use incorrect parameter->max_value
    parse_bytes(UINT8, &buffer, (char *) &default_value);

    break;
  case 11: //folder
    max_value = 0;
    break;
  case 12: //info
    max_value = 0;   

    value = new char[strlen(buffer)+1];
    strlcpy(value, (const char *)buffer,strlen(buffer)+1);
    buffer += strlen(buffer) + 1;
    //db_out.printf("%s",value);

    break;
  case 13: //command
    parse_bytes(UINT8, &buffer, (char *)  &status);
    //parse_bytes(UINT8, &buffer, (char *) &timeout);
    max_value = 0;   
    
    //*(uint8_t *)status = (uint8_t) (*buffer)[0];
    //*buffer += 1;
    timeout = (uint8_t)buffer[0];
    *buffer += 1;

    info = new char[20];
    strlcpy(info, (const char *)buffer, 20);

  default:
      break;
  }
}