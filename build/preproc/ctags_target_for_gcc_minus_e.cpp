# 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp"
# 2 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp" 2
# 3 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp" 2

CurrentSensor::CurrentSensor(int pin, float sensitivity){
    m_pin = pin;
    m_sensitivity = sensitivity;
}

void CurrentSensor::getAdcValue(int _pin){
    analogRead(m_pin); // dummy read
    analogRead(m_pin); // dummy read
    m_adcValue = data_sampling(analogRead(_pin), 10);
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



VoltageDivider solarPanelVoltage(A0, 216000.0, 66500.0);
VoltageDivider batteryVoltage(A1, 217000.0, 66100.0);
// VoltageDivider  loadVoltage(A2, 216000.0, 66100.0); // not used

CurrentSensor solarPanelCurrent(10, 100.0);
CurrentSensor batteryCurrent(11, 100.0);
CurrentSensor loadCurrent(12, 100.0);

RtdSensor batteryTemp(A3, float(20.81)); //offset about 21 ohm
RtdSensor solarPanelTemp(A4, float(28.0)); //ofset about 28 ohm (4 ohm = 1 degree celcius)

DHT OutdoorTempSensor(A6, 11 /**< DHT TYPE 11 */);
WindSensor windSensor(14);
RainSensor Rainsensor(52, 15);


// Function declaration
void serialPrintSensorData(String location, String sensorname, float value, String prefix);
bool serialReadyCheck();
void dataTransfer(int delay);
float stateOfCharge(float voltage, float voltage_max, float voltage_min);

// setup
void setup() {
  Serial.begin(115200);
  OutdoorTempSensor.begin();
  delay(100);
}
// main
void loop(){
  dataTransfer(3000);
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
# 58 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
  // float loadvoltage         = loadVoltage.readVoltage();            delay(10);
  float solarpanelvoltage = solarPanelVoltage.readVoltage(); delay(10);
  float batteryvoltage = batteryVoltage.readVoltage(); delay(10);

  float solarpanelcurrent = solarPanelCurrent.readCurrent(); delay(10);
  float batterycurrent = batteryCurrent.readCurrent(); delay(10);
  float loadcurrent = loadCurrent.readCurrent(); delay(10);

  float batterytemp = batteryTemp.readTemperature(); delay(10);
  float solarpaneltemp = solarPanelTemp.readTemperature(); delay(10);

  float outdoorhumidity = OutdoorTempSensor.readHumidity(); delay(10);
  float outdoortemperature = OutdoorTempSensor.readTemperature(); delay(10);
  float windspeed = windSensor.readWindSpeed(); delay(10);
  int rain = Rainsensor.readRain(); delay(10);

  float SoC = stateOfCharge(batteryVoltage.m_voltage, 12.6, 9.6); delay(10);

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SPV", solarpanelvoltage, "V");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "BV", batteryvoltage, "V");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SoC", SoC, "%");

  // serialPrintSensorData(LOCATION, "LV", loadvoltage, "V");

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SPC", solarpanelcurrent, "A");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "BC", batterycurrent, "A");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "LC", loadcurrent, "A");

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "BT", batterytemp, "celcius");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "SPT", solarpaneltemp, "celcius");

  serialPrintSensorData("Roof" /* Sensorcluster location*/, "OUTHUM", outdoorhumidity, "%");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "OUTTEMP", outdoortemperature, "celcuis");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "windspeed", windspeed, "m/s");
  serialPrintSensorData("Roof" /* Sensorcluster location*/, "RAIN", rain, "HIGH/MEDIUM/LOW");


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

float stateOfCharge(float voltage, float voltage_max, float voltage_min){
  /*

  Calculates state of charge of battery and returns percentage left

  @param voltage is actual voltage level of the battery

  @param voltage_max is the voltage when battery is fully charged

  @param voltage_min is the minimum voltage of battery

  */
# 116 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
  float SoC = ((voltage - voltage_min) / (voltage_max - voltage_min)) * 100;
  if (SoC < 0){
    SoC = 0;
  }
  return SoC;
}
