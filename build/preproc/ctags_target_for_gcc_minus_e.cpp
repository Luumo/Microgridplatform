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

double CurrentSensor::readCurrent(){
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



VoltageDivider solarPanelVoltage(A0, 100000.0, 47000.0);
VoltageDivider batteryVoltage(A1, 100000.0, 47000.0);

CurrentSensor solarPanelCurrent(10);
CurrentSensor batteryCurrent(11);
CurrentSensor loadCurrent(12);

RtdSensor batteryTemp(A3);
RtdSensor solarPanelTemp(A4);

RainSensor Rainsensor(52, 15);
DHT OutdoorTempSensor(A6, 11 /**< DHT TYPE 11 */);
WindSensor windSensor(14);

String my_str = ("stringtest");
/*

Make sure listOfSensors consists of same sensors as dataTransfer function.

each name in listOfSensors should end with a comma, except the last one

  structure:

  "Sensorname1, Sensorname2, Sensorname3,...,SensornameN"

*/
# 35 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
/*

https://www.arduino.cc/reference/tr/language/variables/data-types/string/

*/

char listOfSensors[] = "solarPanelVoltage : V"
                        ","
                        "batteryVoltage : V"
                        ","
                        "solarPanelCurrent : A"
                        ","
                        "batteryCurrent : A"
                        ","
                        "loadCurrent : A"
                        ","
                        "batteryTemp : C"
                        ","
                        "solarPanelTemp : C"
                        ","
                        "rainsensor : true/false"
                        ","
                        "outdoorTemp : C"
                        ","
                        "OutdoorHumidity : %"
                        ","
                        "windSensor : m/s";




void dataTransfer(int delayTime){

  /*

  reads sensordata and sends it via serial communication.

  each value is separated by a comma.

  Decodes on the recieving RPI

  structure:

  clusterlocation,sensortype, value, prefix

  */
# 75 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
  float solarpanelvoltage = solarPanelVoltage.readVoltage();
  float batteryvoltage = batteryVoltage.readVoltage();

  double solarpanelcurrent = solarPanelCurrent.readCurrent();
  double batterycurrent = batteryCurrent.readCurrent();
  double loadcurrent = loadCurrent.readCurrent();

  float batterytemp = batteryTemp.readTemperature();
  float solarpaneltemp = solarPanelTemp.readTemperature();

  int rain = Rainsensor.readRain();
  float outdoorHumidity = OutdoorTempSensor.readHumidity();
  float outdoorTemperature = OutdoorTempSensor.readTemperature();

  Serial.print("Roof" /* Sensorcluster location*/);

  void data_converter(char sensortype, float value, char prefix){
    String str = sensortype + "," + value + "," + prefix;

    print(str )
    }

  Serial.print("Roof" /* Sensorcluster location*/);
  Serial.print(",");
  Serial.print(solarpanelvoltage);
  Serial.print(",");
  Serial.print(batteryvoltage);
  Serial.print(",");
  Serial.print(solarpanelcurrent);
  Serial.print(",");
  Serial.print(batterycurrent);
  Serial.print(",");
  Serial.print(loadcurrent);
  Serial.print(",");
  Serial.print(batterytemp);
  Serial.print(",");
  Serial.print(solarpaneltemp);
  Serial.print(",");
  Serial.print(rain);

  Serial.print(",");
  Serial.print(outdoorHumidity);
  Serial.print(",");
  Serial.print(outdoorTemperature);

  Serial.print(",");
  Serial.print(batterycurrent);
  Serial.print(",");
  Serial.print(batteryvoltage);
  Serial.print("\n");
  delay(delayTime);
}

void CheckConnectionRPI(){
  // wait for serial port to connect.
  while (!Serial) {
  }
}
void setup() {
  Serial.begin(115200);
  OutdoorTempSensor.begin();

  CheckConnectionRPI();
  Serial.print(listOfSensors);
  delay(100);

}

void loop(){

  int solarcurrent = solarPanelCurrent.readCurrent();
  Serial.print(" solarcurrent = " );
  Serial.println(solarcurrent);

  int batterycurrent = batteryCurrent.readCurrent();
  Serial.print(" batterycurrent = " );
  Serial.println(solarcurrent);

  int loadcurrent = loadCurrent.readCurrent();
  Serial.print(" batterycurrent = " );
  Serial.println(solarcurrent);

  delay(1000);

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
