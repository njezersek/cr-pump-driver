#pragma once
#include <SPI.h>
#include "pinout.h"

void mcp4921(uint16_t value){
  uint16_t data = 0x3000 | value;
  digitalWrite(MCP4921_CS_PIN, LOW);
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer((uint8_t)(data >> 8));
  SPI.transfer((uint8_t)(data & 0xFF));
  // SPI.transfer16(value);
  SPI.endTransaction();
  digitalWrite(MCP4921_CS_PIN, HIGH);
}
