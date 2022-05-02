#include "Arduino.h"
#include "rotary_encoder.h"

 
int rotary_encoder_last_pos = 0;

 void setup_encoder() {
    
    ESP32Encoder::useInternalWeakPullResistors=UP;

    encoder.attachSingleEdge(12, 4);

}

uint8_t getRE_POS() {
    
    //delay(100);
    int readEncoder= encoder.getCount();
    int out=0;
    if ( readEncoder > rotary_encoder_last_pos ) {
        //db_out.printf("up :%i\n",readEncoder);
        out =  1;
    }
    if (readEncoder < rotary_encoder_last_pos) {
        //db_out.printf("down :%i\n",readEncoder);
        out = 2;
  } 
	//db_out.printf("Encoder count:%i:%i \n",readEncoder,rotary_encoder_last_pos);
    rotary_encoder_last_pos = readEncoder;

return out;
}

