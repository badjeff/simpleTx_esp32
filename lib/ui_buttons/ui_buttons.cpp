
#include "ui_buttons.h"
#include "crsf_protocol.h"
#include "menus.h"
#include "config.h"
#include "crsf.h"



int selected = 0;
int subSelected = -1;
int mmOptionSelected = -1;
int entered = -1; //-2 idle // -1 main menu // 0 options/submenu
bool menu_loaded = false;

void read_ui_buttons (uint8_t next_param,uint8_t next_chunk) {
    
    bool up = digitalRead(upBt);
    bool down =  digitalRead(downBt);
    bool enter = digitalRead(enterBt);
    bool back = digitalRead(backBt);
    //TODO bt bouncer

    //db_out.printf("%i:%i::%u\n",enter,back,getRE_POS());
   // db_out.printf("%i:%i:%i:\n",selected,subSelected,entered);
    
    if (up == LOW && down == LOW) {
    };
    
    //if at main screen
     if (entered==-2){ 
      //db_out.println("main");
      entered = enter ? entered : -1;

    //if inside main menu - selected = -1
    } else if (entered==-1){
      //db_out.println("menu");

      if ((up == LOW) && (entered != selected))
        //selected = (selected <= 0) ? 0 : selected-1;
      do {
        selected--;
        //db_out.printf("select:%i:\n",selected);
        if (selected < 0) selected = crsf_devices[0].number_of_params-3;
      } while (menuItems[selected].parent != 0); 

      if ((down == LOW) && (entered != selected))
        //selected++;
      do {
        selected++;
        //db_out.printf("select:%i:\n",selected);
        if (selected > crsf_devices[0].number_of_params-3) selected = 0;
      } while (menuItems[selected].parent != 0); 

      if ((enter == LOW)) {
        if ((menuItems[selected].p_type == 9) || ((menuItems[selected].p_type == 13))) {
          entered = -10;
          mmOptionSelected = menuItems[selected].status;
        } else {
          subSelected = selected+1;
          entered = selected;
        }
      }
      if (back == LOW) entered = -2;
    //if at submenu
    } else if (entered == -10) {
    //db_out.printf("select options: %i:%i:%i\n",mmOptionSelected,up,back);
      if (down == 1) {
        //entered = -1;
          if (mmOptionSelected < menuItems[selected].max_value)
            mmOptionSelected++;
          else
            mmOptionSelected = 0;
        }
        if (up == 1) // && (entered != selected))
        {
          //db_out.println("up");
          if (mmOptionSelected > 0) 
            mmOptionSelected--;
          else mmOptionSelected = menuItems[selected].max_value;
        }

      if (back == LOW) entered = -1;
      if (enter == LOW) {
        //db_out.printf("select option %u:%u",mmOptionSelected,selected);
        next_param = selected+1;
        next_chunk = mmOptionSelected;
        Menu::ChangeParam(next_param,next_chunk);
      }


    } else if (entered>=0){
      //db_out.println("@submenu");

      if (enter == LOW) {
        
        next_param = menuItems[subSelected].id;
      
        if (menuItems[subSelected].status < menuItems[subSelected].max_value) {
          next_chunk = menuItems[subSelected].status + 1;
           } else next_chunk = 0;

              
        if (menuItems[subSelected].p_type == 13) next_chunk = 4; //cmd 
        Menu::ChangeParam(next_param,next_chunk);
      }
      if (back == LOW) {
        entered = -1;
        subSelected = -1;
      }
      if ((up == LOW)) // && (entered != selected))
      {
        //db_out.println("up");
        subSelected--;
      }
      if ((down == LOW)) // && (entered != subSelected))
      { 
        //db_out.println("@down");
        subSelected++;
        }

  }

    delay(100);

}