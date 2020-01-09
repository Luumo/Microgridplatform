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

double CurrentSensor::calcCurrentValue(){
    getAdcValue(m_pin);
    calcAdcVoltage();
    currentValue = ((m_adcVoltage - m_offsetVoltage) / m_sensitivity);

    Serial.println("-----Current Sensor -----" );
    Serial.print("Raw Sensor Value = " );
    Serial.println(m_adcValue);
    Serial.print(" Voltage (mV)= " );
    Serial.println(m_adcVoltage);
    Serial.print(" Current = " );
    Serial.print(currentValue);
    Serial.println("\n");
    return currentValue;
}
# 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
# 2 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 3 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 4 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2

# 6 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 7 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 8 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2



CurrentSensor solarPanelCurrent(A10);
CurrentSensor batteryCurrent(A11);
CurrentSensor loadCurrent(A12);

VoltageDivider solarPanelVoltage(A0, 1040000.0, 222500.0);
VoltageDivider batteryVoltage(A1, 1040000.0, 222500.0);
VoltageDivider loadVoltage(A2, 1040000.0, 222500.0);

RtdSensor batteryTemp(A3);
RtdSensor solarPanelTemp(A4);

RainSensor Rainsensor1(52, 15);
DHT OutdoorTempSensor(48, 11 /**< DHT TYPE 11 */);
// WindSensor windsensor(A3);

char listOfSensors[] = "outdoorHumidity,"
                        "outdoortemperature,"
                        "rain,"
                        "batterytemp,"
                        "batterycurrent,"
                        "batteryvoltage";

void dataTransfer(int delayTime);

void setup() {
  Serial.begin(115200);
  OutdoorTempSensor.begin();
  Serial.print(listOfSensors);

}

void loop(){
  dataTransfer(10000);

}

void dataTransfer(int delayTime){
  float outdoorHumidity = OutdoorTempSensor.readHumidity();
  float outdoorTemperature = OutdoorTempSensor.readTemperature();
  int rain = Rainsensor1.SenseRain();

  float batterytemp = batteryTemp.calcTemperature();
  float batterycurrent = batteryCurrent.calcCurrentValue();
  float batteryvoltage = batteryVoltage.calculateVin();

  Serial.print("Roof" /* Sensorcluster location. */);
  Serial.print(",");
  Serial.print(outdoorHumidity);
  Serial.print(",");
  Serial.print(outdoorTemperature);
  Serial.print(",");
  Serial.print(rain);
  Serial.print(",");
  Serial.print(batterytemp);
  Serial.print(",");
  Serial.print(batterycurrent);
  Serial.print(",");
  Serial.print(batteryvoltage);
  Serial.print("\n");
  delay(delayTime);
}


/*

Schema for InfluxDX:

See Line Protocol Syntax for influxDB



data to be sent from arduino:

clusterlocation,sensortype, value, date

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
