# 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp"
# 2 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp" 2

CurrentSensor::CurrentSensor(int pin){
    m_pin = pin;
}

void CurrentSensor::getAdcValue(int _pin){
    m_adcValue = analogRead(_pin);
}

 void CurrentSensor::calcAdcVoltage(){
    m_adcVoltage = (m_adcValue / 1024.0) * 5000; // returns adc voltage
}

float CurrentSensor::readCurrent(){
    getAdcValue(m_pin);
    calcAdcVoltage();
    currentValue = ((m_adcVoltage - m_offsetVoltage) / m_sensitivity);
    return currentValue;
}
# 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
# 2 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 3 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 4 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 5 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2

# 7 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 8 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 9 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2

# 11 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2



VoltageDivider solarPanelVoltage(A0, 215000.0, 66500.0);
VoltageDivider batteryVoltage(A1, 217000.0, 66100.0);

CurrentSensor solarPanelCurrent(10);
CurrentSensor batteryCurrent(11);
CurrentSensor loadCurrent(12);

RtdSensor batteryTemp(A3, float(21)); //offset about 21 ohm
RtdSensor solarPanelTemp(A4, float(28)); //ofset about 28 ohm (4 ohm = 1 degree celcius)

RainSensor Rainsensor(52, 15);
DHT OutdoorTempSensor(A6, 11 /**< DHT TYPE 11 */);
WindSensor windSensor(14);

// Function declaration
void serialPrintSensorData(String location, String sensorname, float value, String prefix);
bool serialReadyCheck();
void dataTransfer(int delay);

// setup
void setup() {
  Serial.begin(115200);
  OutdoorTempSensor.begin();
  // serialReadyCheck();
  delay(100);
}
// main
void loop(){
  dataTransfer(2000);
}

// Functions
void dataTransfer(int delayTime){

  /*

  reads sensordata and sends it via serial communication.

  each value is separated by a comma.

  Decodes on the recieving RPI

  structure:

  clusterlocation, sensortype, value, prefix

  */
# 55 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
  float solarpanelvoltage = solarPanelVoltage.readVoltage(); delay(100);
  float batteryvoltage = batteryVoltage.readVoltage(); delay(100);

  float solarpanelcurrent = solarPanelCurrent.readCurrent(); delay(100);
  float batterycurrent = batteryCurrent.readCurrent(); delay(100);
  float loadcurrent = loadCurrent.readCurrent(); delay(100);

  float batterytemp = batteryTemp.readTemperature(); delay(100);
  float solarpaneltemp = solarPanelTemp.readTemperature(); delay(100);

  int rain = Rainsensor.readRain(); delay(100);
  float outdoorhumidity = OutdoorTempSensor.readHumidity(); delay(100);
  float outdoortemperature = OutdoorTempSensor.readTemperature(); delay(100);
  float windspeed = windSensor.readWindSpeed(); delay(100);

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SPV", solarpanelvoltage, "V");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "BV", batteryvoltage, "V");

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SPC", solarpanelcurrent, "A");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "BC", batterycurrent, "A");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "LC", loadcurrent, "A");

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "BT", batterytemp, "celcius");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SPT", solarpaneltemp, "celcius");

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "RAIN", rain, "HIGH/MEDIUM/LOW");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "OUTHUM", outdoorhumidity, "%");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "OUTTEMP", outdoortemperature, "celcuis");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "windspeed", windspeed, "m/s");

  delay(delayTime);
}

void serialPrintSensorData(String location, String sensorname, float value, String prefix){
  Serial.print(location);
  Serial.print(",");
  Serial.print(sensorname);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(prefix);
  Serial.print("\n");
}

bool serialReadyCheck(){
  // Wait until RPI is ready
  while (1){
    if (Serial.available()){
      Serial.write("Check");
      String data = Serial.readStringUntil('\n');
      if (data.equals("RPIready")){
        return true;
        break;
      }
    }
  }
}

  /*

  Schema for InfluxDX:

  See Line Protocol Syntax for influxDB

  handled at raspberry:

    date, time



  data to be sent from arduino:

    clusterlocation,sensortype, value



  ->

  this has to be parsed to python:

  data = [

    {

      "measurement": sensorcluster,

      "tags": {

        "location": location_var,

        "sensortype": sensortype_Var

      },

      "time": iso,

      "fields": {

        "value": sensorvalue_Var

      }

    }

  ]





  **SORTED BY SENSORCLUSTER**

  ex:



  INSERT sensorcluster,clusterlocation=1,sensortype=outdoortemp value=23 XXXXXXXXXXXX 

  see Milk-sensor documentation for futher info about data protocol from python



  name: Sensorcluster

  -----------------

  time                         sensortype       clusterlocation    value

  2019-12-11T19:30:00.xxxxxx   Humidity         1                    90

  2019-12-11T19:30:00.xxxxxx   outdoortemp      1                    22

  2019-12-11T19:30:00.xxxxxx   batterytemp      1                    23

  2019-12-11T19:30:00.xxxxxx   batteryCurrent   1                    8

  2019-12-11T19:30:00.xxxxxx   batteryvoltage   1                    12.3

  2019-12-11T19:30:00.xxxxxx   PVvoltage        1                    12.3

  2019-12-11T19:30:00.xxxxxx   PVCurrent        1                    8





*/
