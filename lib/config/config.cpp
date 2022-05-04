#include "Arduino.h"
#include "config.h"
#include "crsf.h"


uint32_t clickCurrentMicros = 0;


const char *hdr_str_cb(const void *data) {
    
    (void)data;
     //   db_out.printf("call params: %u: %i\n",count_params_loaded(), device_idx);

    if (count_params_loaded() != crsf_devices[device_idx].number_of_params) {
   //     db_out.printf("not all params: %u: %i\n",count_params_loaded(), device_idx);
    
        snprintf(tempstring, sizeof tempstring, "%s %s", crsf_devices[device_idx].name, "LOADING");
    
    } else if (protocol_module_is_elrs()) {
        db_out.printf("idx_elrs: %i\n",device_idx);

        snprintf(tempstring, sizeof tempstring, "%s  %d/%d  %c",
                 crsf_devices[device_idx].name, elrs_info.bad_pkts, elrs_info.good_pkts,
                 (elrs_info.flags & 1) ? 'C' : '-');
    } else  {
        db_out.printf("tx module \n not detected\n");
        //return crsf_devices[device_idx].name;
        snprintf(tempstring, sizeof tempstring, "%s  %d/%d  %c",
                 crsf_devices[device_idx].name, elrs_info.bad_pkts, elrs_info.good_pkts,
                 (elrs_info.flags & 1) ? 'C' : '-');
    }
    return tempstring;
}

void crsfdevice_init() {
    next_param = 1;
    next_chunk = 0;
    params_loaded = 0;
    
    //CBUF_Init(send_buf);
}
void bt_handle(uint8_t value) {
  db_out.println("bt_handle");
  
  powerChangeHasRun=true;   
  clickCurrentMicros = crsfTime + 500000;//0.5sec
  db_out.printf("times: %u:%u\n", clickCurrentMicros/1000, crsfTime/1000);
  //powerChangeHasRun=true;
  
  //CRSF_read_param(crsfCmdPacket,1,next_chunk);
  //elrsWrite(crsfCmdPacket,8,0);
  
  //buildElrsPingPacket(crsfCmdPacket);
  //db_out.println(CRSF_send_model_id(2));
  
  //set modelId
  //CRSF_sendId(crsfSetIdPacket,0);
  //elrsWrite(crsfSetIdPacket,LinkStatisticsFrameLength);

  //turn on rx wifi, even if missmatch modelId
  //buildElrsPacket(crsfCmdPacket,16,1);

  CRSF_read_param(crsfCmdPacket,1,next_chunk);
  elrsWrite(crsfCmdPacket,8,200000);
  //serialEvent();
}
