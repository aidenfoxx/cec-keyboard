#include <DigiKeyboard.h>

#include "src/tiny-cec/cec.h"

#define CEC_PIN 5

#define CEC_MSG_USER_CONTROL_PRESSED 0x44
#define CEC_MSG_USER_CONTROL_RELEASED 0x45
#define CEC_MSG_GIVE_OSD_NAME 0x46
#define CEC_MSG_SET_OSD_NAME 0x47
#define CEC_MSG_SET_STREAM_PATH 0x86
#define CEC_MSG_ACTIVE_SOURCE 0x82
#define CEC_MSG_GIVE_PHYSICAL_ADDR 0x83
#define CEC_MSG_REPORT_PHYSICAL_ADDR 0x84
#define CEC_MSG_GIVE_DEVICE_VENDOR_ID 0x8c
#define CEC_MSG_DEVICE_VENDOR_ID 0x87
#define CEC_MSG_GIVE_DEVICE_POWER_STATUS 0x8f
#define CEC_MSG_REPORT_POWER_STATUS 0x90
#define CEC_MSG_CEC_VERSION 0x9e
#define CEC_MSG_GET_CEC_VERSION 0x9f

#define CEC_BROADCAST_ADDR 0xf
#define CEC_VERSION_1_3A 0x04

const unsigned char PHYSICAL_ADDR[2] = {0x13, 0x37};
const unsigned char VENDOR_ID[3] = {0, 0x13, 0x37};
const char *OSD_NAME = "HTPC";

void transmitPhysicalAddr() {
  unsigned char data[] =
    {CEC_MSG_REPORT_PHYSICAL_ADDR, PHYSICAL_ADDR[0], PHYSICAL_ADDR[1], 0x04};
  cec_transmitMsg(CEC_BROADCAST_ADDR, data, sizeof(data));
}

void transmitActiveSource(unsigned char *data) {
  if (data[1] == PHYSICAL_ADDR[0] && data[2] == PHYSICAL_ADDR[1]) {
    unsigned char data[] = {CEC_MSG_ACTIVE_SOURCE, PHYSICAL_ADDR[0], PHYSICAL_ADDR[1]};
    cec_transmitMsg(CEC_BROADCAST_ADDR, data, sizeof(data));
  }
}

void transmitPowerStatus() {
  unsigned char data[] = {CEC_MSG_REPORT_POWER_STATUS, 0}; // Powered on
  cec_transmitMsg(0, data, sizeof(data));
}

void transmitCECVersion() {
  unsigned char data[] = {CEC_MSG_REPORT_POWER_STATUS, CEC_VERSION_1_3A};
  cec_transmitMsg(0, data, sizeof(data));
}

void transmitOSDName() {
  unsigned char data[] = {CEC_MSG_SET_OSD_NAME, VENDOR_ID[0], VENDOR_ID[1], VENDOR_ID[2]};
  cec_transmitMsg(0, data, sizeof(data));
}

void transmitVendorId() {
  unsigned char data[sizeof(OSD_NAME) + 1] = {CEC_MSG_DEVICE_VENDOR_ID};
  memcpy(data + 1, OSD_NAME, sizeof(OSD_NAME));
  cec_transmitMsg(0, data, sizeof(data));
}

void handleKeyDown(int key) {

}

void handleKeyUp() {
  
}

void setup() {
  cec_init(CEC_PIN, CDT_PLAYBACK_DEVICE);
}

void loop() {
  unsigned char *data;
  int count;
  
  if (cec_readMsg(&data, &count, false)) {
    return;
  }

  switch (data[0]) {
    case CEC_MSG_GIVE_PHYSICAL_ADDR: transmitPhysicalAddr(); break;
    case CEC_MSG_SET_STREAM_PATH: transmitActiveSource(data); break;
    case CEC_MSG_GIVE_DEVICE_POWER_STATUS: transmitPowerStatus(); break;
    case CEC_MSG_GET_CEC_VERSION: transmitCECVersion(); break;
    case CEC_MSG_GIVE_OSD_NAME: transmitOSDName(); break;
    case CEC_MSG_GIVE_DEVICE_VENDOR_ID: transmitVendorId(); break;
    case CEC_MSG_USER_CONTROL_PRESSED: handleKeyDown(data[1]); break;
    case CEC_MSG_USER_CONTROL_RELEASED: handleKeyUp(); break;
  }
}
