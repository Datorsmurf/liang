#include <Arduino.h>
#include <Logger.h>
#include <settings.h>
SETTINGS::SETTINGS() {
}  

void SETTINGS::setup(LOGGER *logger_) {
logger = logger_;
}

void SETTINGS::commit() {
      EEPROM.commit();
}

String SETTINGS::readWifiSid(){
  return  EEPROM.readString(EEPROM_ADR_WIFI_SSID);
}
void SETTINGS::writeWifiSid(String v) {
  EEPROM.writeString(EEPROM_ADR_WIFI_SSID, v);
}
String SETTINGS::readWifiPwd(){
  return  EEPROM.readString(EEPROM_ADR_WIFI_PWD);
}
void SETTINGS::writeWifiPwd(String v) {
  EEPROM.writeString(EEPROM_ADR_WIFI_PWD, v);
}

uint16_t SETTINGS::readInitialMode(){
  uint16_t result = EEPROM.readUShort(EEPROM_ADR_INIT_MODE);
  return  result <= MAX_MANUAL_OPMODE ? result : OP_MODE_IDLE;
}

void SETTINGS::writeInitialMode(uint16_t v) {
  EEPROM.writeUShort(EEPROM_ADR_INIT_MODE, v);
}

uint16_t SETTINGS::readWheelMotorLoadLimit(){
  uint16_t result = EEPROM.readUShort(EEPROM_ADR_WHEEL_LOAD_LIMIT);
  return result > 0 ? result : LOAD_LIMIT_WHEEL;
}

void SETTINGS::writeWheelMotorLoadLimit(uint16_t v) {
  EEPROM.writeUShort(EEPROM_ADR_WHEEL_LOAD_LIMIT, v);
}

uint16_t SETTINGS::readCutterLoadLimit(){
  uint16_t result = EEPROM.readUShort(EEPROM_ADR_CUTTER_LOAD_LIMIT);
  return result > 0 ? result : LOAD_LIMIT_CUTTER;
}

void SETTINGS::writeCutterLoadLimit(uint16_t v) {
  EEPROM.writeUShort(EEPROM_ADR_CUTTER_LOAD_LIMIT, v);
}