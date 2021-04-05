#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define         BOOL_PIN                     (2)
#define         DHTPIN                        (8)    
#define         DHTTYPE DHT22                        
#define         MG_PIN                       (A0)
#define         DC_GAIN                      (8.5)   
#define         READ_SAMPLE_INTERVAL         (50)    
#define         READ_SAMPLE_TIMES            (5)     
#define         ZERO_POINT_VOLTAGE           (0.220) 
#define         REACTION_VOLTGAE             (0.030) 
#define         SensorPin 0
#define         WATER_TEMP_PIN                (A4)   
const float VRefer = 3.3;                            
const int pinAdc   = A2;                             
float CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};
OneWire oneWire(WATER_TEMP_PIN); 
DallasTemperature sensors(&oneWire);
float sensorValue = 0;
DHT sensor(DHTPIN, DHTTYPE);
const int chipSelect = 4;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
void setup() {
  Serial.begin(9600);
  while (!Serial) {    
  }  
  Ethernet.begin(mac, ip);
  sensor.begin();
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("Initializing SD card...");
if (!SD.begin(chipSelect)) {
Serial.println("Card failed, or not present");
return;
}
Serial.println("card initialized.");
pinMode(BOOL_PIN, INPUT);                        
digitalWrite(BOOL_PIN, HIGH);                    
}
float MGRead(int mg_pin)
{
    int i;
    float v=0;
    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *5/1024 ;
    return v;
}
int  MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else {
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
}
float readO2Vout()
{
    long sum = 0;
    for(int i=0; i<32; i++) {
        sum += analogRead(pinAdc);
    }
    sum >>= 5;
    float MeasuredVout = sum * (VRefer / 1023.0);
    return MeasuredVout;
}
float readConcentration()
{
    float MeasuredVout = readO2Vout();
    float Concentration = MeasuredVout * 0.21 / 2.0;
    float Concentration_Percentage=Concentration*100;
    return Concentration_Percentage;
}
char get_CO2()
{
    int percentage;
    float volts;
    volts = MGRead(MG_PIN);
    percentage = MGGetPercentage(volts,CO2Curve);
    return char(percentage);
}
char get_O2()
{
     float Vout =0;
     Vout = readO2Vout();
     float concentration = readConcentration();
     return char(concentration);
}

void loop() {
  float humidity = sensor.readHumidity(); 
  float temperature_C = sensor.readTemperature();
  float temperature_F = sensor.readTemperature(true);
  float heat_indexF = sensor.computeHeatIndex(temperature_F, humidity);
  float heat_indexC = sensor.convertFtoC(heat_indexF); 
  float CO2 = get_CO2();
  float O2 = get_O2();  
File sdcard_file = SD.open("data.txt", FILE_WRITE);
if (sdcard_file) {
sdcard_file.print("Temperature in C: ");
sdcard_file.print(temperature_C);
sdcard_file.print("  Temperature in Fah: ");
sdcard_file.print(temperature_F);
sdcard_file.print("  Humidity: ");
sdcard_file.print(humidity);
sdcard_file.print("  Heat Index in F: ");
sdcard_file.print(heat_indexF);
sdcard_file.print("  Heat Index in C: ");
sdcard_file.println(heat_indexC);
sdcard_file.print("  CO2: ");
sdcard_file.println(CO2);
sdcard_file.print("  O2 : ");
sdcard_file.println(O2);
sdcard_file.close();
}  
else {
Serial.println("error opening data.txt");
} 
delay(2000);
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 10");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
            client.println("</h4><h4>L'humidite :");
            client.print(humidity);client.print(" %");
            client.println("</h4><h4>La temperature :");
            client.print(temperature_C);client.print(" C");
            client.println("</h4><h4>CO2 :");
            client.print(CO2);client.print(" ppm");
            client.println("</h4><h4>CO2 :");
            client.print(O2);client.print(" %");
            client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1000);
    client.stop();
    Serial.println("client disconnected");
  }
}
