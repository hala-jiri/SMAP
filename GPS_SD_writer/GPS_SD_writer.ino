#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <Wire.h> 

#define TX 3
#define RX 4
const int chipSelect = 10;
File myFile;
String filename = "GPS_positions.txt";

TinyGPS gps;
SoftwareSerial sw(RX, TX);

void setup() {
  Serial.begin(9600);
  sw.begin(9600);
  
   Serial.print("Initializing SD card...");
    if (!SD.begin()) 
    {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
}

void loop() 
{
  bool receivedNewData = false;

  for (unsigned long start = millis(); millis() - start < 1000;) 
  {
    while (sw.available()) 
    {
      char c = sw.read();
      if (gps.encode(c)) 
      {
        receivedNewData = true;
      }
    }
  }
  if (receivedNewData) 
  {

    float Latitude, Longitude;
    unsigned long age, date, time, chars;
    int year;
    byte month, day, hour, minute, second, miliseconds;
    char DateAndTimeString[20];
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &miliseconds, &age);

    sprintf(DateAndTimeString, "[%4d-%02d-%02d %02d:%02d:%02d]", year, month, day, hour, minute, second);
    
    Serial.println("File opening..");
    myFile = SD.open(filename.c_str(), FILE_WRITE);
    if (myFile) 
    {    

        Serial.println("File open");
        myFile.print(DateAndTimeString);
        myFile.print(" ");
        
        gps.f_get_position(&Latitude, &Longitude);
    
        myFile.print("Lat:");
        myFile.print(Latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : Latitude, 6);
        myFile.print(";Lon:");
        myFile.print(Longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : Longitude, 6);
        myFile.print(";Alt:");
        myFile.print(gps.f_altitude() == TinyGPS::GPS_INVALID_F_ALTITUDE ? 0 : gps.f_altitude());
        
        myFile.print(";Speed:");
        myFile.print(gps.f_speed_kmph() == TinyGPS::GPS_INVALID_F_SPEED ? 0 : gps.f_speed_kmph());
        myFile.print(";Sat:");
        myFile.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    
        myFile.print("Prec:");
        myFile.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
        myFile.close();
        Serial.println("File close");
    }
  }
}
