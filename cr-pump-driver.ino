#include <HX711.h>

#include "pinout.h"
#include "mc4921.h"

HX711 scale;

uint32_t start, stop;


void setup(){
  pinMode(MCP4921_CS_PIN, OUTPUT);
  digitalWrite(MCP4921_CS_PIN, HIGH);
  SPI.begin();

  Serial.begin(9600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(237872.0f / 600.0f); // uncalibrated scale.get_units(10) / <actual wetight>
  scale.tare();
}


uint16_t value = 0;

void loop(){
  // float v = (1.0+sin((float) micros() / 200000.0))/2. * 4096;
  // uint16_t value = (uint16_t) v;

  if(scale.is_ready()){
    float s = scale.get_units();
    s = (s-300)/(460-300); // 300 - 460
    Serial.println(s);
    if(s <= 0.0f) s = 0.0f;
    if(s >= 1.0f) s = 1.0f;
    value = (uint16_t) (s * 4095.0f);
  }
  mcp4921(value);

}