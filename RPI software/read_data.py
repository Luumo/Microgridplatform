import serial
from influxdb import InfluxDBClient
import time

client = InfluxDBClient(host='localhost', port=8086, username='luan', password='microgrid')
client.switch_database('mydb')
interval = 20

measurement = "sensorcluster"

def parse_data(line):
    location, sensortype, value, prefix = line.strip().split(",") # split at "," and strips \n
    value = float(value)
    data = [
        {
        "measurement": measurement,
        "tags":{
            "location": location,
            "sensortype": sensortype,
            },
        #"time": timestamp,
        "fields":{
            "sensorvalue": value,
            }
        }
    ]
    return data


ser = serial.Serial('/dev/ttyS0',115200)
time.sleep(3)
while 1:
    if(ser.in_waiting > 0 ):
        line = ser.readline().decode() # decodes bytes to str
        data = parse_data(line) #parses string to line_protocol for influxDB
        print(data)
        #client.write_points(data)
        #time.sleep(interval)







