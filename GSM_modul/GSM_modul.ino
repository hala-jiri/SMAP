#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
char msg;

// atributy, ktere nahrazují funkci více zapojených modulů (vstupů)
bool alarm = true;
bool imobilizer = true;


String messageTemplate = "https://www.google.com/maps/search/?api=1&query=";
String lat = "51.xxx348";
String lon = "5.xxx445";

char c;
String cString;
String phoneNumber;
  
void setup()
{
  mySerial.begin(9600);   
  Serial.begin(9600);
  delay(100);
}

void loop()
{  
  if (Serial.available()>0)
   switch(Serial.read())
  {
    if(alarm)
    {
      SendGpsPositionMessage();
    }
    // read a message
    ReadMessages();
  }
 if (mySerial.available()>0)
 Serial.write(mySerial.read());
}

void ReadMessages()
{
  mySerial.println("AT+CNMI=2,2,0,0,0");
  
  delay(1000);
}

void SendGpsPositionMessage()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+420776xxxx\"\r");
  delay(500);
  mySerial.println(messageTemplate + lat +","+ lon);
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}

void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (mySerial.available()>0)
  {
    c=mySerial.read();
    cString += c;
    if (c=='\n')
    {
      
      if (cString.substring(0,2)=="OK") 
      {
        return;
        
      }
      if(cString.substring(0,5)=="+CMGR")
      {
        //prvni + "+CMGR"
        int cIndex = cString.indexOf('+');
        if(cIndex < 0) 
        {
          return;
        }
       
        cString = cString.substring(cIndex+1);
        cIndex = cString.indexOf('+');
        phoneNumber = cString.substring(cIndex+4,cIndex+13);
        if(phoneNumber.indexOf("77667xxxx")>0)
        {
          // zpráva je z mého Čísla
          imobilizer = true;
          alarm = true;
          SendGpsPositionMessage();
        }
      }
  }
}
}
