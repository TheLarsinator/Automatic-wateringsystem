#include <SD.h>

const int valvePin = 9;
const int sensorPin = A0;
const int sensorPowerPin = 2;
const int ledPin = 7;

int switchState = 0;
bool valveOpen = false;
bool updateValve = false;
int sensorValue = 0;

File statFile;
File valveState;
void toggleValve();

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting setup");
  //Set all pins to correct modes
  pinMode(valvePin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(sensorPowerPin, OUTPUT);

  Serial.println("Starting SD card");

   pinMode(10, OUTPUT);
   pinMode(ledPin, OUTPUT);

  if (!SD.begin(4)) {
   Serial.println("initialization failed!");
   return;
 }
 Serial.println("initialization done.");

  statFile = SD.open("stats.csv", FILE_WRITE);
  statFile.println("time(s);value");
  statFile.close();
  Serial.println("Setup complete!");
}

void toggleValve()
{
  valveOpen = !valveOpen;
  if(valveOpen)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(valvePin, HIGH);
    Serial.println("Opening valve!");
  }
  else
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(valvePin, LOW);
    Serial.println("Closing valve!");
  }
}

void loop()
{
    digitalWrite(sensorPowerPin, HIGH);
    delay(100);
    sensorValue = analogRead(sensorPin);
    delay(100);
    digitalWrite(sensorPowerPin, LOW);
    
    sensorValue = 1023-sensorValue;
    
    statFile = SD.open("stats.csv", FILE_WRITE);
    delay(100);

    unsigned long times = static_cast<unsigned long>(millis());
    times /= static_cast<unsigned long>(1000);
    
    statFile.print(times);
    statFile.print(";");
    statFile.println(sensorValue);
    Serial.print(times);
    Serial.print(";");
    Serial.println(sensorValue);
    delay(100);
    statFile.close();

    if(sensorValue < 550)
    {
      toggleValve();
      valveState = SD.open("valve.csv", FILE_WRITE);
      valveState.print("Opened valve at ");
      valveState.println(times);
      valveState.close();
      delay(10000);
      toggleValve();
    }
   delay(60000);
}
