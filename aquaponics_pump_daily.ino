/*Created by: Aaron Snowden
  Date: 5/1/2014
  This code is to be uploaded on the microcontroller
  This code does the following:
	-Flashes led warnings if the moisture or temperature sensors merit warning
	-When a command is given through the serial port at 9600 baud rate the arduino:
		Responds to a call for a report by sending all available sensor data
		Responds to an acknowledgement call
		Turns on a normally OPEN switch for the water pump
		Turns on a normally open switch for the lights
 
  All sensor report data is immediate except the plantTemp and lampTemp
      -plant and lamp temp keep a running avg and that value is cleared after a report is requested
*/

#include <dht.h>

//
//the defined number corresponds to the placement on the arduino
/////Sensors/////

//DIGITAL
#define MOISTURE_DIGITAL 10
#define DEPTH_TRIGGER 12
#define DEPTH_ECHO 11

#define PUMPSWITCH 7
#define SWITCH_TIME 8 //seconds
#define LIGHTSWITCH 13 

//ANALOG
#define LIGHT 4 //ANALOG READ
#define PLANT_TEMP 3 //ALSO ANALOG READ 
#define MOISTURE_ANALOG 2

/////LED/////
#define BLUE 5
#define GREEN 4
#define RED 3

////GLOBAL VARIABLES///
dht LAMP_DHT;
int dht_dpin = 2; // this is digital!
int minL, maxL, light;

/////////////////
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
float lampTempSum = 0;
float plantTempSum = 0;
int secondsOfHour = 0;
int hour = 1;

//this function is used by the raspberry pi and for testing
//it is called once a report is asked for through the serial port
//values are sent from each of the six sensor's corresponding function and seperated by a comma
int sendReport(){
  int value = 0;
  
  //value 1
  value = avgPlantTemp();
  Serial.print(value);
  Serial.print(",");
  
  //value 2
  value = depth();
  Serial.print(value);
  Serial.print(",");
  
  //value 3
  value = lampHumidity();
  Serial.print(value);
  Serial.print(",");
  
  //value 4
  value = avgLampTemp();
  Serial.print(value);
  Serial.print(",");
  
  //value 5
  value = lightRead();
  Serial.print(value);
  Serial.print(",");

  //value 6
  value = moisture();
  Serial.print(value);
  //Serial.print(","); 
}

//this is for a normally open pump
//the digital write turns the pump on for the number of seconds given in the form of an int to the function
//the first digitalWrite to the pumpswitch is in the main loop
int pump(int seconds)
{
  digitalWrite(PUMPSWITCH, HIGH);
  delay(seconds*1000);
  digitalWrite(PUMPSWITCH, LOW);
  Serial.print("PUMP INITIATED");
}


///SENSORS REPORTS///
//value 1 TEMPERATURE
//this value is obtained with the lm35 temperature sensor
 int plantTemp(){
 int reading = analogRead(PLANT_TEMP);
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 // now print out the temperature //modified: for lm35 degrees = mV /10 NOT (mV - 500)/10
 int temperatureC = voltage * 100;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 // now convert to Fahrenheit
 int temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 //Serial.print(temperatureF); //Serial.println(" degrees F");
 return temperatureF;  
}
  
//value 2 DEPTH
//this value is obtained using a algebraic distance equation
int depth(){ //in milli
  unsigned long t, t_start, c;
  // Flush sensor LOW for a second
  digitalWrite(DEPTH_TRIGGER, LOW);
  delayMicroseconds(2);
  
  // Pulse for 10 us
  digitalWrite(DEPTH_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(DEPTH_TRIGGER, LOW);
  
  // Wait until echo signal is up.
  c = 0;
  while (digitalRead(DEPTH_ECHO) == LOW && c++ < 10000) {}
  
  // Measure echo signal uptime in microseconds. Also blink LED pin 13 during
  c = 0;
  t_start = micros();
  while(digitalRead(DEPTH_ECHO) == HIGH && c++ < 10000) {}
  
  // Duration in microseconds
  t = micros() - t_start;
  
  // Distance in meters = (duration in seconds) * (speed of sound m/s) / 2
  // Distance in cm = (t * 1e-6) * (340 * 1e2) / 2 = t * 17/1000
  // Distance in millimeters = (t * 1e-6) * (340 * 1e3) / 2 = t * 17/10
  
  // Return distance in mm, sensor is supposedly accurate to 0.3cm = 3mm
  return t * 17 / 100;
}
//value 3 HUMIDITY
//this value is created using the DHT sensor and the DHT dynamicaly linked library
int lampHumidity(){
  LAMP_DHT.read11(dht_dpin);
  return LAMP_DHT.humidity;
}
//value 4 TEMPERATURE
//this value is created using the DHT sensor and the DHT dynamically linked library
int lampTemp(){
  LAMP_DHT.read11(dht_dpin);
  int temperatureF = (LAMP_DHT.temperature * 9.0 / 5.0) + 32.0;
  return temperatureF;
}
//value 5 LIGHT reading
//this value is based on an analog read and fluctuates a notable amount
int lightRead(){
   light = analogRead(LIGHT);
   light = light*5;
   //int adjustedLight = map(light, minL, maxL, 0, 100);
   return light;
}

//value 6 MOISTURE levels
//this value corresponds to the moisture levels from the moisture sensor
//it is an analog value and requires an analog read
int moisture()
{ 
  /*
 moisture level description
 0 - 300 dry soil
 300 - 700 humid soil
 700 - 950 in water (or air)
 */
 
 int moisture;
 moisture = analogRead(MOISTURE_ANALOG);
 return moisture;
}

//call in main
int sumPlantTemp(){
  plantTempSum += (float)plantTemp();
}
//call on request
int avgPlantTemp(){
  int avg = plantTempSum/secondsOfHour;
  return avg;
}
//call in main
int sumLampTemp(){
  lampTempSum += (float)lampTemp();
}
//call on request
int avgLampTemp(){
  int avg = lampTempSum/secondsOfHour;
  return avg;
}
void ledack()
{
  for(int cnt = 3; cnt > 0; cnt--)
  {
  digitalWrite(GREEN, HIGH);
    delay(200);
   digitalWrite(GREEN, LOW);
   digitalWrite(RED,HIGH);
    delay(200);
   digitalWrite(RED, LOW);
   digitalWrite(BLUE,HIGH);
    delay(200);
   digitalWrite(BLUE, LOW);
  }
  
 Serial.print("ACK REQUEST RECEIVED"); 
}
void lightOn()
{
  digitalWrite(LIGHTSWITCH, LOW);
  Serial.print("LIGHTON");
}
void lightOff()
{
  digitalWrite(LIGHTSWITCH, HIGH);
  Serial.print("LIGHTOFF");
}
void lightAdjust(int light, int minL , int maxL)
{
  light = analogRead(LIGHT);
  if(minL>light)
    minL = light;
  if(maxL<light)
    maxL = light;
}

void lightSettings()
{
  float hr = hour;
    //for light switch
  if(secondsOfHour == 1)
  {
    if(hr == 5.0)
    {
     lightOn();
    }
    else if(hr == 7.0)
    {
      lightOff();
    }
    else if(hr == 8.0)
    {
      lightOn();
    }
    else if(hr == 12.0)
    {
      lightOff();
    }
    else if(hr == 18.0)
    {
       lightOn();
    }
    else if(hr == 17.0)
    {
        lightOff();
    }
    else if(hr == 18.0)
    {
       lightOn();
    }
    else if(hr == 22.0)
    {
        lightOff();
    }
  }
}


void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  //set up pins
  pinMode(DEPTH_TRIGGER, OUTPUT);
  pinMode(DEPTH_ECHO, INPUT);
  pinMode(PUMPSWITCH, OUTPUT);
  pinMode(LIGHTSWITCH, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(MOISTURE_DIGITAL, INPUT);
  
  digitalWrite(PUMPSWITCH, LOW);

  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  //settings and control statement for lights
  lightSettings();
   
   if(hour == 1 && secondsOfHour == 10)
   {
    int n = SWITCH_TIME;
    Serial.println("pump's on");
     pump(n);
     secondsOfHour += n;
   }
   
   sumLampTemp();
   sumPlantTemp();
      
   if(secondsOfHour == 60)
   { 
     secondsOfHour = 0;
     hour = (hour % 24) + 1;
     
     Serial.println("Data Cleared");
     lampTempSum = 0;
     plantTempSum = 0;
   }
   secondsOfHour = (secondsOfHour % 3600) + 1;
   Serial.println(secondsOfHour);
  //wait a second
  delay(1000);
}
void serialEvent()
{
  while (Serial.available()) {
    ////////////////////
    //READ//
    // get the new byte:    
    char inChar = (char)Serial.read(); 

    inputString += inChar;
    
    if (inChar == '\n')
    {
    stringComplete = true;
    //Serial.println("string received");
    }
    //SEND REPORT
    if ((stringComplete)&&(inputString == "REPORT\n"))
    {
      Serial.println("REPORTACK");
      sendReport();
      //order matters
      Serial.println();
    }
    //LED CONNECTION ACK FLASH
    if ((stringComplete)&&(inputString == "LEDACK\n"))
    {
      ledack();
      Serial.println();
    }
    //PUMPSWITCH
    if ((stringComplete)&&(inputString == "PUMP\n"))
    {
      pump(SWITCH_TIME); //run pumpswitch, check code failure        
      Serial.println();
    }
    //LIGHT SWITCH ON
    if ((stringComplete)&&(inputString == "LIGHTON\n"))
    {
      lightOn(); //run pumpswitch, check code failure        
      Serial.println();
    }
    //
    if ((stringComplete)&&(inputString == "LIGHTOFF\n"))
    {
      lightOff(); //run pumpswitch, check code failure        
      Serial.println();
    }
    //Serial.println("current string: " + inputString);
  } 
}

