#include <arduino.h>
#include "Digitalpin.h"
#include "Voltagedivider.h"
#include "Currentsensor.h"
#include "Windsensor.h"
#include "RtdSensor.h"



CurrentSensor Currentsensor1(A7);
VoltageDivider voltagedivider1(A1, 1040000.0, 222500.0);
// WindSensor windsensor(A3);
RtdSensor RtdSensor1(A3);


void setup() {          
  Serial.begin(115200);
  
}

void loop() {

  Serial.print("Temp: ");
  Serial.println();
  Serial.print("Humidity: ");
  Serial.println();
  //Currentsensor1.calcCurrentValue();
  //voltagedivider1.calculateVin();

  RtdSensor1.calcTemperature();
  delay(1000);
}
