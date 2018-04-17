#include <SoftwareSerial.h>

#define aref_voltage 3.3 // tie 3.3V to ARef

// Right Subwoofer, clockwise, begins in upper right corner
#define Right1R 3
#define Right1G 4
#define Right1B 5

#define Right2R 6
#define Right2G 7
#define Right2B 8

#define Right3R 9
#define Right3G 10
#define Right3B 11

#define Right4R 12
#define Right4G 13
#define Right4B 14

// Left Subwoofer, clockwise, begins in upper right corner
#define Left1R 15
#define Left1G 16
#define Left1B 17

#define Left1R 18
#define Left1G 19
#define Left1B 20

#define Left1R 21
#define Left1G 22
#define Left1B 23

#define Left1R 24
#define Left1G 25
#define Left1B 26

#define ModeSwitch 28 // Button for switching profile

#define TempSensor A2 // Temperature sensor for the amplifiers

#define FanRegulator 29 // Manual regulator for the fans
#define FanSpeed A3     // connect the fans here

#define Mic A1 //Input from the microphone

SoftwareSerial mySerial(30, 31);

byte SoundIntensity; // Variable to hold the intensity of the analog sound signal

// Variables for the button
byte buttonPushCounter = 0; // Counter for the number of button presses
byte buttonState = 0;       // Current state of the button
byte lastButtonState = 0;   // Previous state of the button

// Variables for the temp sensor and fans
int currentTemp;    // Stores the current temperature
int manualFanSpeed; // Holds the reading from the fanregulator

byte LeftRandom; // Holds random numbers for the left / right speaker
byte RightRandom;

unsigned long previousMillis = 0; // time since last run of the code

// constants won't change:
const long interval = 1000; // interval at which to do TempCheck (milliseconds)

byte pins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}; // the pins to of the LEDs

String string;
char command;

string pinvalue

    void
    setup()
{
  Serial.begin(57600); // Serial used for bluetooth
  // The data rate for the SoftwareSerial port needs to
  // match the data rate for your bluetooth board.
  mySerial.begin(9600);

  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);

  pinMode(ModeSwitch, INPUT);
  pinMode(TempSensor, INPUT);
  pinMode(FanRegulator, INPUT);
  pinMode(Mic, INPUT);

  pinMode(FanSpeed, OUTPUT);

  //make Right & Left SB pins outputs
  for (int i = 0; i < sizeof(pins); i++)
  {
    pinMode(pins[i], OUTPUT); //makes all LED pins outputs
  }
}

void massAnalogWrite()
{
  for (int i = 0; i < sizeof(pins); i++)
  {
    digitalWrite(pins[i], pinvalue); //turns off all LEDs
  }
}

void Bluetooth()
{
  // if there's a new command reset the string
  if (mySerial.available())
  {
    string = "";
  }

  // Construct the command string fetching the bytes, sent by Android, one by one.
  while (mySerial.available())
  {
    command = ((byte)mySerial.read()); // Reads the serial port and adds the data to the byte "command"

    if (command == ':') // Stops if a ":" is encounted
    {
      break;
    }
    else
    {
      string += command; // Puts the recived commands into string commands
    }
    // delay(1);
  }

  Serial.println(string); // Print on the Monitor latest command recieved
}

void LEDconf()
{
  if (string == "All OFF")
  {
    // Turn off all LEDs
    massAnalogWrite();
  }
  else if (string == "RED ON")
  {
    // Turn on LED red
    digitalWrite(Right1R, HIGH);
  }
  else if (string == "GREEN ON")
  {
    // Turn on LEF green
    digitalWrite(Right1G, HIGH);
  }
  else if (string == "GREEN OFF")
  {
    // Turn off LED green
    digitalWrite(Right1G, LOW);
  }
  else if (string == "BLUE ON")
  {
    // Turn on LED blue
    digitalWrite(Right1B, HIGH);
  }
  else if (string == "BLUE OFF")
  {
    // Turn off LED blue
    digitalWrite(Right1B, LOW);
  }

  /* if (string.startsWith("#"))
  {
    String value = string.substring(1);
    if (value.startsWith("RED"))
    {
      value = value.substring(3);
      analogWrite(Right1R, value.toInt());
    }
    else if (value.startsWith("GREEN"))
    {
      value = value.substring(5);
      analogWrite(Right1G, value.toInt());
    }
    else if (value.startsWith("BLUE"))
    {
      value = value.substring(4);
      analogWrite(Right1B, value.toInt());
    } */
}
}

void TempCheck()
{
  // call this to check the temps and control the fans,
  //comment this part out if you dont have a temp sensor - fan control setup.

  // checks if the manual override regulator has been used. If not, this code runs (automatic fan control)
  manualFanSpeed = analogRead(FanRegulator); // reads the setting of the fan regulator and stores it in manualFanSpeed

  if (manualFanSpeed = 0)
  {
    currentTemp = analogRead(TempSensor); // values range from 0 -1023

    // converting that reading to voltage, which is based off the reference voltage
    float voltage = currentTemp * aref_voltage;
    voltage /= 1024.0;

    //converting from 10 mv per degree wit 500 mV offset to degrees ((volatge - 500mV) times 100)
    float temperatureC = (voltage - 0.5) * 100;

    if (temperatureC >= 19)
    { //turns of the fans at 19 degress Celsius or below
      analogWrite(FanSpeed, 0);
    }
    else if (temperatureC >= 20)
    { // fans at 5% at 20c or under
      analogWrite(FanSpeed, 50);
    }
    else if (temperatureC >= 25)
    { // ~20% fanspeed at 25c
      analogWrite(FanSpeed, 250);
    }
    else if (temperatureC >= 30)
    { // 40% fanspeed at 30c
      analogWrite(FanSpeed, 400);
    }
    else if (temperatureC >= 35)
    { // 60% fanspeed at 35c
      analogWrite(FanSpeed, 600);
    }
    else if (temperatureC >= 45)
    { // 80% fanspeed at 40c
      analogWrite(FanSpeed, 800);
    }
    else if (temperatureC >= 50)
    { //fans at full speed at 50 degress Celsius
      analogWrite(FanSpeed, 1023);
    }
  }
}

void SpinEffect() //creates a infinte loopable spin effect
{
  digitalWrite(Right4, HIGH);
  digitalWrite(Left4, HIGH);
  digitalWrite(Right2, LOW);
  digitalWrite(Left2, LOW);

  digitalWrite(Right1, HIGH);
  digitalWrite(Left1, HIGH);
  digitalWrite(Right3, LOW);
  digitalWrite(Left3, LOW);

  digitalWrite(Right2, HIGH);
  digitalWrite(Left2, HIGH);
  digitalWrite(Right4, LOW);
  digitalWrite(Left4, LOW);

  digitalWrite(Right3, HIGH);
  digitalWrite(Left3, HIGH);
  digitalWrite(Right1, LOW);
  digitalWrite(Left1, LOW);

  digitalWrite(Right4, HIGH);
  digitalWrite(Left4, HIGH);
  digitalWrite(Right2, LOW);
  digitalWrite(Left2, LOW);
}

void FlashingSound() // flashes the LEDs in beat with the music
{
  //reads the sound signal and saves it as a number between 0 and 1023
  SoundIntensity = analogRead(Mic);

  // Makes all the LEDs flash with the music. Adjust the number with * for better precision
  for (int i = 0; i < sizeof(pins); i++)
  {
    digitalWrite(pins[i], SoundIntensity * 15); //assigns the pin the value of variable "SoundIntensity";
  }
}

void RandomEffect() // Lights the LEDs in a random sequense
{
  //Creates 3 byte values and generates random values
  LeftRandom = random(4);
  RightRandom = random(4);
  LightRandom = random(3)

      massDigitalWrite(pinvalue = LOW); // Släcker alla lamporna

if ((currentMillis - previousMillis >= interval)
{
    if (LeftRandom = 1)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Left1R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Left1G, LightRandom);
      }
      else
      {
        digitalWrite(Left1B, LightRandom);
      }
    }
    if (LeftRandom = 2)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Left2R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Left2G, LightRandom);
      }
      else
      {
        digitalWrite(Left2B, LightRandom);
      }
    }
    if (LeftRandom = 3)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Left3R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Left3G, LightRandom);
      }
      else
      {
        digitalWrite(Left3B, LightRandom);
      }
    }
    if (LeftRandom = 4)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Left4R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Left4G, LightRandom);
      }
      else
      {
        digitalWrite(Left4B, LightRandom);
      }
    }
}
  if ((currentMillis - previousMillis >= interval)
{
    if (RightRandom = 1)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Right1R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Right1G, LightRandom);
      }
      else
      {
        digitalWrite(Right1B, LightRandom);
      }
    }

    if (RightRandom = 2)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Right2R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Right2G, LightRandom);
      }
      else
      {
        digitalWrite(Right2B, LightRandom);
      }
    }

    if (RightRandom = 3)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Right3R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Right3G, LightRandom);
      }
      else
      {
        digitalWrite(Right3B, LightRandom);
      }
    }

    if (RightRandom = 4)
    {
      if (LightRandom == 1)
      {
        digitalWrite(Right4R, LightRandom);
      }
      else if (LightRandom == 2)
      {
        digitalWrite(Right4G, LightRandom);
      }
      else
      {
        digitalWrite(Right4B, LightRandom);
      }
    }
 }
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval || command == "TEMP OFF:")
  {
    // save the last time the code ran
    previousMillis = currentMillis;

    TempCheck();
  }

  Bluetooth();

  LEDconf();
}
