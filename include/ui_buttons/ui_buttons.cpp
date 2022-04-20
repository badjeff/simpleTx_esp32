
#include "rotary_encoder/rotary_encoder.cpp"


void read_ui_buttons () {
    

    bool up = false; //digitalRead(upBt);
    bool down = false;// digitalRead(downBt);
    bool enter = digitalRead(enterBt);
    bool back = digitalRead(backBt);
    //TODO bt bouncer

    //db_out.printf("%i:%i::%u\n",enter,back,getRE_POS());
   // db_out.printf("%i:%i:%i:\n",selected,subSelected,entered);
    
    uint8_t tmp = getRE_POS(); 

    if (tmp==1) up = 1;
    else if (tmp==2) down = 1;
    else {
      up = 0;
      down = 0;
    }

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
        db_out.printf("click:%i:%i:%s:%u:%u\n",
        selected,
        menuItems[selected].parent,
        menuItems[selected].name,
        menuItems[selected].status,
        menuItems[selected].max_value
        );

        if ((menuItems[selected].p_type == 9) 
            || ((menuItems[selected].p_type == 13))) {
        db_out.printf("find:%i:%s:%u:%u\n",
        selected,
        menuItems[selected].name,
        menuItems[selected].status,
        menuItems[selected].max_value);
        //params_loaded = 0;
        entered = -10;
        mmOptionSelected = menuItems[selected].status;

        //Oled::selectOptionMainMenu();

        /* if (menuItems[selected].status < menuItems[selected].max_value) {
          next_chunk = menuItems[selected].status + 1;
           } else next_chunk = 0;

        next_param = menuItems[selected].id;
        if (menuItems[selected].p_type == 13) next_chunk = 4; //cmd 

        //next_chunk == cmd to send
        Menu::ChangeParam(next_param,next_chunk); */
        

        } else {
            db_out.printf("not find:%i:%s\n",
                selected,
                menuItems[selected].name);

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
        db_out.printf("select option %u:%u",mmOptionSelected,selected);
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

        db_out.printf("send cmd submenu \n %i:%i:%u:%u:%u:type:%u:%u\n",
              selected,
              subSelected,
              menuItems[subSelected].id,
              menuItems[subSelected].max_value,
              menuItems[subSelected].status,
              menuItems[subSelected].p_type,
              next_chunk);
        
              
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


    //db_out.printf("ent:%i:sel:%isSel:%i\n",entered, selected,subSelected);
    
    //powerChangeHasRun=true;
    //clickCurrentMicros = crsfTime + (2*1000000);//2sec
    //delay(200);
    delay(100);

}