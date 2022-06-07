#include "Arduino.h"
#include "config.h"
#include "uart.h"
#include "gpio.h"
#include "crsf.h"

#if defined(USE_USB_HID_INPUT_CH)
  #include "usb_host_hid_bridge.h"
  void config_desc_cb(const usb_config_desc_t *config_desc);
  void device_info_cb(usb_device_info_t *dev_info);
  void hid_report_descriptor_cb(usb_transfer_t *transfer);
  void hid_report_cb(usb_transfer_t *transfer);
  UsbHostHidBridge hidBridge;
  int32_t usb_input_ch[] = { 0,0,0,0, 0,0,0,0 };

#endif

// IO setup

int upBt = 12;
int downBt = 4;
int enterBt = 14;
int backBt = 5;

void initGpio()
{

  // analogReference(EXTERNAL);
  #if defined(USE_GPIO_INPUT_CH)
    pinMode(ANALOG_IN_PIN_AUX1, INPUT_PULLDOWN);
    pinMode(ANALOG_IN_PIN_AUX2, INPUT_PULLUP);
  #endif
  // pinMode(DIGITAL_PIN_LED, OUTPUT);//LED
  // pinMode(DIGITAL_PIN_BUZZER, OUTPUT);//
  // digitalWrite(DIGITAL_PIN_BUZZER, LOW);
  // batteryVoltage=7.0;

  pinMode(upBt, INPUT_PULLUP);
  pinMode(downBt, INPUT_PULLUP);
  pinMode(enterBt, INPUT_PULLUP);
  pinMode(backBt, INPUT_PULLUP);
  pinMode(DigitalInPinPowerChange, INPUT_PULLUP);

  #if defined(TARGET_ESP32_S)
    analogReadResolution(12); // 4096
  #endif

  #if defined(USE_USB_HID_INPUT_CH)
    hidBridge.setOnConfigDescriptorReceived( config_desc_cb );
    hidBridge.setOnDeviceInfoReceived( device_info_cb );
    hidBridge.setOnHidReportDescriptorReceived( hid_report_descriptor_cb );
    hidBridge.setOnReportReceived( hid_report_cb );
    hidBridge.begin();
  #endif
}

#if defined(USE_GPIO_INPUT_CH)

void gpioReadInputs(rc_input_t* rc_input)
{
    rc_input->aileron  = analogRead(ANALOG_IN_PIN_AILERON);
    rc_input->elevator = analogRead(ANALOG_IN_PIN_ELEVATOR);
    rc_input->throttle = analogRead(ANALOG_IN_PIN_THROTTLE);
    rc_input->rudder   = analogRead(ANALOG_IN_PIN_RUDDER);
    rc_input->aux1     = analogRead(ANALOG_IN_PIN_AUX1);
    rc_input->aux2     = analogRead(ANALOG_IN_PIN_AUX2);
    rc_input->aux3     = analogRead(ANALOG_IN_PIN_AUX3);
    rc_input->aux4     = analogRead(ANALOG_IN_PIN_AUX4);
}

void gpioMixer(rc_input_t* rc_input)
{
    rc_input->aux1 = (rc_input->aux1 >= 2047) ? 4095 : 0;
}

#elif defined(USE_USB_HID_INPUT_CH)

void gpioReadInputs(rc_input_t* rc_input)
{
    rc_input->aileron  = usb_input_ch[0];
    rc_input->elevator = usb_input_ch[1];
    rc_input->throttle = usb_input_ch[2];
    rc_input->rudder   = usb_input_ch[3];
    rc_input->aux1     = 0;
    rc_input->aux2     = 0;
    rc_input->aux3     = 0;
    rc_input->aux4     = 0;
}

void gpioMixer(rc_input_t* rc_input)
{
}

void config_desc_cb(const usb_config_desc_t *config_desc) {
    usb_print_config_descriptor(config_desc, NULL);
}

void device_info_cb(usb_device_info_t *dev_info) {
    if (dev_info->str_desc_manufacturer) usb_print_string_descriptor(dev_info->str_desc_manufacturer);
    if (dev_info->str_desc_product)      usb_print_string_descriptor(dev_info->str_desc_product);
    if (dev_info->str_desc_serial_num)   usb_print_string_descriptor(dev_info->str_desc_serial_num);
}

void hid_report_descriptor_cb(usb_transfer_t *transfer) {
    //>>>>> for HID Report Descriptor
    // Explanation: https://electronics.stackexchange.com/questions/68141/
    // USB Descriptor and Request Parser: https://eleccelerator.com/usbdescreqparser/#
    //<<<<<
    dbout.printf("\nstatus %d, actual number of bytes transferred %d\n", transfer->status, transfer->actual_num_bytes);
    for(int i=0; i < transfer->actual_num_bytes; i++) {
        if (i == USB_SETUP_PACKET_SIZE) {
            dbout.printf("\n\n>>> Goto https://eleccelerator.com/usbdescreqparser/ \n");
            dbout.printf(">>> Copy & paste below HEX and parser as... USB HID Report Descriptor\n\n");
        }
        dbout.printf("%02X ", transfer->data_buffer[i]);
    }
    dbout.printf("\n\n");
    // dbout.printf("HID Report Descriptor\n");
    uint8_t *const data = (uint8_t *const)(transfer->data_buffer + USB_SETUP_PACKET_SIZE);
    size_t len = transfer->actual_num_bytes - USB_SETUP_PACKET_SIZE;
    // dbout.printf("> size: %ld bytes\n", len);
    bool isGamepad = false;
    bool isVenDef  = false;
    if (len >= 5) {
        uint8_t gamepadUsagePage[] = { 0x05, 0x01, 0x09, 0x05 };
        uint8_t vdrDefUsagePage[] = { 0x06, 0x00, 0xFF, 0x09, 0x01 };
        isGamepad = memcmp(data, gamepadUsagePage, sizeof(gamepadUsagePage)) == 0;
        isVenDef  = memcmp(data, vdrDefUsagePage, sizeof(vdrDefUsagePage)) == 0;
    }
    dbout.printf(">>> best guess: %s\n", isGamepad ? "HID Gamepad" : isVenDef ? "Vendor Defined" : "Unkown");
}

void hid_report_cb(usb_transfer_t *transfer) {
    //
    // check HID Report Descriptor for usage
    // gist: https://gist.github.com/jledet/2857343
    //       https://www.microchip.com/forums/m913995.aspx
    //
    unsigned char *const data = (unsigned char *const)(transfer->data_buffer);
    // for (int i=0; i<transfer->actual_num_bytes && i<11; i++) {
    //     for (int b = 8; b != -1; b--) printf("%d", (data[i] & (1 << b)) >> b );
    //     printf(" ");
    // }
    // printf("\n");
    usb_input_ch[0] = data[0] * 16; //map(data[0], 0, 255, 0, 4096);
    usb_input_ch[1] = data[1] * 16; //map(data[1], 0, 255, 0, 4096);
    usb_input_ch[2] = data[2] * 16; //map(data[2], 0, 255, 0, 4096);
    usb_input_ch[3] = data[3] * 16; //map(data[3], 0, 255, 0, 4096);
    for (int i=0; i<4; i++) dbout.printf("%d ", usb_input_ch[i]);
    dbout.printf("\n");
}

#endif
