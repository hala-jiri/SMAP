const int PIEZO_PIN = A5; 
void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  int analogValue = analogRead(PIEZO_PIN);
  Serial.println(analogValue);
  delay(250);
}
