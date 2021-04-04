#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define         BOOL_PIN                     (2)
#define         DHTPIN                        (8)    // Pin DHT22
#define         DHTTYPE DHT22                        // DHT 22  (AM2302)
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
  // start the server
  sensor.begin();
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("Initializing SD card...");
if (!SD.begin(chipSelect)) {
Serial.println("Card failed, or not present");
// don't do anything more:
return;

}

Serial.println("card initialized.");
                              //UART setup, baudrate = 9600bps
pinMode(BOOL_PIN, INPUT);                        //set pin to input
digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors
}


void loop() {
  float humidity = sensor.readHumidity(); 
  float temperature_C = sensor.readTemperature();
  float temperature_F = sensor.readTemperature(true);
  float heat_indexF = sensor.computeHeatIndex(temperature_F, humidity);
  float heat_indexC = sensor.convertFtoC(heat_indexF);

File sdcard_file = SD.open("data.txt", FILE_WRITE);
// if the file is available, write to it:
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
sdcard_file.print("  Capteur CO2: ");

sdcard_file.close();
// print to the serial port too:
Serial.print(" Temperature in C: ");
Serial.println(temperature_C);
Serial.print(" Temperature in Fah: ");
Serial.println(temperature_F);
Serial.print(" Humidity: ");
Serial.println(humidity);
Serial.print(" Heat Index in F: ");
Serial.println(heat_indexF);
Serial.print(" Heat Index in C: ");
Serial.println(heat_indexC);
Serial.print(" concentration de O2: ");

}  
// if the file isn't open, pop up an error:
else {
Serial.println("error opening data.txt");
} 

delay(2000);


  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 10");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //Affichage des valeurs
            client.println("</h4><h4>L'humidite :");
            client.print(humidity);client.print(" %");
            client.println("</h4><h4>La temperature :");
            client.print(temperature_C);client.print(" C");
            client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1000);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
