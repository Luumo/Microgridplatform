#include <arduino.h>
#include <string.h>
#include "DHT.h"
#include "Voltagedivider.h"
#include "Currentsensor.h"
#include "Windsensor.h"
#include "RtdSensor.h"
#include "RainSensor.h"
#include "Windsensor.h"
#include "Sampling.h"

#define LOCATION "Roof"    // Sensorcluster location

VoltageDivider  solarPanelVoltage(A0, 216000.0, 66500.0);
VoltageDivider  batteryVoltage(A1, 217000.0, 66100.0);
// VoltageDivider  loadVoltage(A2, 216000.0, 66100.0); // not used

CurrentSensor   solarPanelCurrent(10, 100.0);
CurrentSensor   batteryCurrent(11, 100.0);
CurrentSensor   loadCurrent(12, 100.0);

RtdSensor       batteryTemp(A3, float(20.81)); //offset about 21 ohm
RtdSensor       solarPanelTemp(A4, float(28.0)); //ofset about 28 ohm (4 ohm = 1 degree celcius)

DHT             OutdoorTempSensor(A6, DHT11);
WindSensor      windSensor(14);
RainSensor      Rainsensor(48, 15);

float solarpanelvoltage;
float solarpanelcurrent;
float solarpaneleffect;

float batteryvoltage;
float batterycurrent;
float batteryeffect;

float SoC;

float loadeffect;
float loadcurrent;   

float batterytemp;      
float solarpaneltemp;

float outdoorhumidity;
float outdoortemperature;
float windspeed;
int   rain;      

// temp variables for sampling sensative data
float solarpanelvoltage_temp;
float batteryvoltage_temp;
float solarpanelcurrent_temp;
float batterycurrent_temp;
float loadcurrent_temp;  

//Timer declaration sensitive data
unsigned long previousTimeSensitive = 0;
const unsigned long SensitiveInterval = 3000;
float numSamples = 0.0;
//Timer declaration non sensitive data
unsigned long previousTime = 0;
const unsigned long interval = 2000;

//Timer declaration send serial data
unsigned long SendDatapreviousTime = 0;

// Function declaration
void SerialProtocol(String location, String sensorname, float value, String prefix);
void dataTransfer(int delay); // remove

void collectsensitiveData(unsigned long currentTime);
void collectNonSensitiveData(unsigned long currentTime);

float stateOfCharge(float voltage, float voltage_max, float voltage_min);
float calcEffect(float voltage, float ampere);
void convertTemporaryData();

// setup
void setup() {          
  Serial.begin(115200);
  OutdoorTempSensor.begin();
  delay(100);
}


// main
void loop(){

  unsigned long currentTime = millis();
  collectsensitiveData(currentTime);
  collectNonSensitiveData(currentTime);

  if ((currentTime - SendDatapreviousTime) >= interval){
    serialPrintData();
    SendDatapreviousTime = currentTime;
  }


}

// Functions
void serialPrintData(){
  // print sensitive data
  SerialProtocol(LOCATION, "SPV", solarpanelvoltage, "V");
  SerialProtocol(LOCATION, "BV", batteryvoltage, "V");

  SerialProtocol(LOCATION, "SPC", solarpanelcurrent, "A");
  SerialProtocol(LOCATION, "BC", batterycurrent, "A");
  SerialProtocol(LOCATION, "LC", loadcurrent, "A");

  SerialProtocol(LOCATION, "SPE", solarpaneleffect, "W");
  SerialProtocol(LOCATION, "BE", batteryeffect, "W");
  SerialProtocol(LOCATION, "LE", loadeffect, "w");

  // print non sensitive data
  SerialProtocol(LOCATION, "BT", batterytemp, "celcius");
  SerialProtocol(LOCATION, "SPT", solarpaneltemp, "celcius");

  SerialProtocol(LOCATION, "OUTHUM", outdoorhumidity, "%");
  SerialProtocol(LOCATION, "OUTTEMP", outdoortemperature, "celcuis");
  SerialProtocol(LOCATION, "windspeed", windspeed, "m/s");
  SerialProtocol(LOCATION, "RAIN", rain, "0-1023");
  SerialProtocol(LOCATION, "SoC", SoC, "%");
}

void collectsensitiveData(unsigned long currentTime){
  if (currentTime - previousTimeSensitive >= SensitiveInterval) {
    convertTemporaryData();
    solarpaneleffect  = calcEffect(solarpanelvoltage, solarpanelcurrent);
    batteryeffect     = calcEffect(batteryvoltage, batterycurrent);
    loadeffect        = calcEffect(5.0, loadcurrent);
    SoC               = stateOfCharge(batteryvoltage, 12.6, 9.6);

    previousTimeSensitive = currentTime;

  }
     
  else{
    solarpanelvoltage_temp    += solarPanelVoltage.readVoltage();
    batteryvoltage_temp       += batteryVoltage.readVoltage();
    solarpanelcurrent_temp    += solarPanelCurrent.readCurrent();
    batterycurrent_temp       += batteryCurrent.readCurrent();
    loadcurrent_temp          += loadCurrent.readCurrent();
    numSamples += 1.0;
  }
}


void collectNonSensitiveData(unsigned long currentTime){
  if ((currentTime - previousTime) >= interval){
    batterytemp         = batteryTemp.readTemperature();
    solarpaneltemp      = solarPanelTemp.readTemperature();

    outdoorhumidity     = OutdoorTempSensor.readHumidity();
    outdoortemperature  = OutdoorTempSensor.readTemperature();
    windspeed           = windSensor.readWindSpeed();
    rain                = Rainsensor.readRain();

    previousTime = currentTime;
  }
}


void dataTransfer(int delayTime){

  /*
  reads sensordata and sends it via serial communication.
  each value is separated by a comma.
  Decodes on the recieving RPI
  structure:
  clusterlocation, sensortype, value, prefix
  */

  solarpanelvoltage   = solarPanelVoltage.readVoltage();
  batteryvoltage      = batteryVoltage.readVoltage();

  solarpanelcurrent   = solarPanelCurrent.readCurrent();
  batterycurrent      = batteryCurrent.readCurrent();
  loadcurrent         = loadCurrent.readCurrent();

  batterytemp         = batteryTemp.readTemperature();
  solarpaneltemp      = solarPanelTemp.readTemperature();

  outdoorhumidity     = OutdoorTempSensor.readHumidity();
  outdoortemperature  = OutdoorTempSensor.readTemperature();
  windspeed           = windSensor.readWindSpeed();
  rain                  = Rainsensor.readRain();

  SoC = stateOfCharge(batteryVoltage.m_voltage, 12.6, 9.6);

  SerialProtocol(LOCATION, "SPV", solarpanelvoltage, "V");
  SerialProtocol(LOCATION, "BV", batteryvoltage, "V");
  SerialProtocol(LOCATION, "SoC", SoC, "%");

  SerialProtocol(LOCATION, "SPC", solarpanelcurrent, "A");
  SerialProtocol(LOCATION, "BC", batterycurrent, "A");
  SerialProtocol(LOCATION, "LC", loadcurrent, "A");

  SerialProtocol(LOCATION, "BT", batterytemp, "celcius");
  SerialProtocol(LOCATION, "SPT", solarpaneltemp, "celcius");

  SerialProtocol(LOCATION, "OUTHUM", outdoorhumidity, "%");
  SerialProtocol(LOCATION, "OUTTEMP", outdoortemperature, "celcuis");
  SerialProtocol(LOCATION, "windspeed", windspeed, "m/s");
  SerialProtocol(LOCATION, "RAIN", rain, "HIGH/MEDIUM/LOW");


  delay(delayTime);
}

void SerialProtocol(String location, String sensorname, float value, String prefix){
  Serial.print(location);
  Serial.print(",");
  Serial.print(sensorname);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(prefix);
  Serial.print("\n");
}

float stateOfCharge(float voltage, float voltage_max, float voltage_min){
  /*
  Calculates state of charge of battery and returns percentage left
  @param voltage is actual voltage level of the battery
  @param voltage_max is the voltage when battery is fully charged
  @param voltage_min is the minimum voltage of battery
  */
  float SoC = ((voltage - voltage_min) / (voltage_max - voltage_min)) * 100;
  if (SoC < 0){
    SoC = 0;
  }
  return SoC; 
}

float calcEffect(float voltage, float ampere){
  return voltage*ampere;
}

void convertTemporaryData(){
  /*
  converts sampled data and resets temp variable and numSamples
  */
  solarpanelvoltage = solarpanelvoltage_temp/numSamples;
  batteryvoltage    = batteryvoltage_temp/numSamples;

  solarpanelcurrent = solarpanelcurrent_temp/numSamples;
  batterycurrent    = batterycurrent_temp/numSamples;
  loadcurrent       = loadcurrent_temp/numSamples;

  solarpanelvoltage_temp = 0;
  batteryvoltage_temp = 0;

  solarpanelcurrent_temp = 0;
  batterycurrent_temp = 0;
  loadcurrent_temp = 0;
    
  numSamples = 0.0;
}