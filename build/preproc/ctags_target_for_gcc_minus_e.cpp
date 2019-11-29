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

    Serial.print("Raw Sensor Value = " );
    Serial.print(m_adcValue);
    Serial.print(" Voltage (mV)= " );
    Serial.print(m_adcVoltage);
    Serial.print(" Current = " );
    Serial.print(currentValue);
    Serial.print("\n");
    return currentValue;
}
# 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
# 2 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 3 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 4 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2

# 6 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 7 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2
# 8 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino" 2

dht DHT;



CurrentSensor Currentsensor1(A7);
VoltageDivider voltagedivider1(A1, 1040000.0, 222500.0);
// WindSensor windsensor(A3);
RtdSensor RtdSensor1(A3);


void setup() {
  Serial.begin(115200);

}

void loop() {

  DHT.read11(48);
  Serial.print("Temp: ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity: ");
  Serial.println(DHT.humidity);
  //Currentsensor1.calcCurrentValue();
  //voltagedivider1.calculateVin();

  RtdSensor1.calcTemperature();
  delay(1000);
}
