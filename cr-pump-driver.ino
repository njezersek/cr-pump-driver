#include "pinout.h"
#include "mc4921.h"


uint32_t start, stop;


void setup(){
  pinMode(MCP4921_CS_PIN, OUTPUT);
  digitalWrite(MCP4921_CS_PIN, HIGH);
  SPI.begin();
}


void loop(){
  float v = (1.0+sin((float) micros() / 200000.0))/2. * 4096;
  uint16_t value = (uint16_t) v;
  mcp4921(value);
}