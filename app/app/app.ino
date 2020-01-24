#include <SoftwareSerial.h>

SoftwareSerial EEBlue(0, 1); // RX | TX

int LED_RED_STATUS = 0;
int LED_GREEN_STATUS = 0;
int LED_YELLOW_STATUS = 0;

int LED_RED_PIN = 12;
int LED_GREEN_PIN = 11;
int LED_YELLOW_PIN = 10;

int LED_RED_TIMER = 0;
int LED_YELLOW_TIMER = 1000;
int LED_GREEN_TIMER = 1000;

unsigned long red_time_now = 0;
unsigned long yellow_time_now = 0;
unsigned long green_time_now = 0;

String RED_ALWAYS_ON_STR = "r11";
String RED_10_SEC_STR = "r10";
String RED_15_SEC_STR = "r15";
String RED_OFF_STR = "r00";

String GREEN_ALWAYS_ON_STR = "g11";
String GREEN_10_SEC_STR = "g10";
String GREEN_15_SEC_STR = "g15";
String GREEN_OFF_STR = "g00";

String YELLOW_ALWAYS_ON_STR = "y11";
String YELLOW_10_SEC_STR = "y10";
String YELLOW_15_SEC_STR = "y15";
String YELLOW_OFF_STR = "y00";

String SYNC_STR = "get";

char junkData;
String dataFromApp = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
}

void toggleRedLED(int state) {
  if (state == 1) {
    digitalWrite(LED_RED_PIN, HIGH);
    LED_RED_STATUS = 1;
    dataFromApp = "";

    
    if (LED_RED_TIMER != 0)
    {
      red_time_now = millis();
    }
    
  }
  else {
    digitalWrite(LED_RED_PIN, LOW);

    LED_RED_STATUS = 0;
  }
}


void autoOffRedLEDCheck()
{
  if(LED_RED_STATUS == 1 && LED_RED_TIMER != 0) {
    if(millis() > red_time_now + LED_RED_TIMER)
    {
        digitalWrite(LED_RED_PIN, LOW);
  
        LED_RED_STATUS = 0;
  
        getLEDStatus();
    }
  }
}

void toggleYellowLED(int state) {
  if (state == 1) {
    digitalWrite(LED_YELLOW_PIN, HIGH);
    LED_YELLOW_STATUS = 1;
    dataFromApp = "";
    
    if (LED_YELLOW_TIMER != 0)
    {
      yellow_time_now = millis();
    }
  }
  else {
    digitalWrite(LED_YELLOW_PIN, LOW);

    LED_YELLOW_STATUS = 0;
  }
}

void autoOffYellowLEDCheck()
{
  if(LED_YELLOW_STATUS == 1 && LED_YELLOW_TIMER != 0) {
    if(millis() > yellow_time_now + LED_YELLOW_TIMER)
    {
        digitalWrite(LED_YELLOW_PIN, LOW);
  
        LED_YELLOW_STATUS = 0;
  
        getLEDStatus();
    }
  }
}


void toggleGreenLED(int state) {
  if (state == 1) {
    digitalWrite(LED_GREEN_PIN, HIGH);
    LED_GREEN_STATUS = 1;
    dataFromApp = "";
    
    if (LED_GREEN_TIMER != 0)
    {
      green_time_now = millis();
    }
  }
  else {
    digitalWrite(LED_GREEN_PIN, LOW);

    LED_GREEN_STATUS = 0;
  }
}

void autoOffGreenLEDCheck()
{
  if(LED_GREEN_STATUS == 1 && LED_GREEN_TIMER != 0) {
    if(millis() > green_time_now + LED_GREEN_TIMER)
    {
        digitalWrite(LED_GREEN_PIN, LOW);
  
        LED_GREEN_STATUS = 0;
  
        getLEDStatus();
    }
  }
}

void clearJunkBuffer()
{
  while (Serial.available() > 0)
    {
      junkData = Serial.read() ;  // clear the serial buffer
    }
}

bool isValidCommChar(char ch)
{
  bool result = (ch == 'r' || ch == '0' || ch == '1' || ch == 'g' || ch == 'y' || ch == '5' || ch == 'e' || ch == 't') ;
  return result;
}

void getLEDStatus() { 
  //RGY format 010 means green alone is on
  String ledStatus = "";
  if (LED_RED_STATUS == 1) {
    ledStatus = ledStatus + '1';
  }
  else {
    ledStatus = ledStatus + '0';
  }

  if (LED_GREEN_STATUS == 1) {
    ledStatus = ledStatus + ' ' + '1';
  }
  else {
    ledStatus = ledStatus + ' ' + '0';
  }

  if (LED_YELLOW_STATUS == 1) {
    ledStatus = ledStatus + ' ' + '1';
  }
  else {
    ledStatus = ledStatus + ' ' + '0';
  }

  //Data will be read by android
  Serial.println("ok " + ledStatus);
  
}

void loop() {
  
  autoOffRedLEDCheck();
  autoOffYellowLEDCheck();
  autoOffGreenLEDCheck();

  if (Serial.available()) {    
    while (Serial.available())
    {
      char incomingData = (char)Serial.read(); //read the input
      //Serial.println(incomingData);
      
      if (dataFromApp.length() >= 3) 
      {
        dataFromApp = "";
      }
      
      if (isValidCommChar(incomingData) == true)
      {
        dataFromApp += incomingData;  
        //Serial.println(dataFromApp);//make a string of the characters coming on serial
      }
    }

    clearJunkBuffer();

    if (dataFromApp == RED_10_SEC_STR) {
      //Serial.println("turn on led red for 10 secs");
      LED_RED_TIMER = 10000;
      toggleRedLED(1);
    }
    else if (dataFromApp == RED_15_SEC_STR) {
      //Serial.println("turn on led red for 15 secs");
      LED_RED_TIMER = 15000;
      toggleRedLED(1);
    }
    else if (dataFromApp == RED_ALWAYS_ON_STR) {
      //Serial.println("turn on led red always");
      LED_RED_TIMER = 0;
      toggleRedLED(1);
    }
    else if (dataFromApp == RED_OFF_STR) {
      LED_RED_TIMER = 0;
      toggleRedLED(0);
    }
    else if (dataFromApp == GREEN_10_SEC_STR) {
      //Serial.println("turn on led green for 10 secs");
      LED_GREEN_TIMER = 10000;
      toggleGreenLED(1);
    }
    else if (dataFromApp == GREEN_15_SEC_STR) {
      //Serial.println("turn on led green for 15 secs");
      LED_GREEN_TIMER = 15000;
      toggleGreenLED(1);
    }
    else if (dataFromApp == GREEN_ALWAYS_ON_STR) {
      //Serial.println("turn on led green always");
      LED_GREEN_TIMER = 0;
      toggleGreenLED(1);
    }
    else if (dataFromApp == GREEN_OFF_STR) {
      LED_GREEN_TIMER = 0;
      toggleGreenLED(0);
    }
    else if (dataFromApp == YELLOW_10_SEC_STR) {
      //Serial.println("turn on led yellow for 10 secs");
      LED_YELLOW_TIMER = 10000;
      toggleYellowLED(1);
    }
    else if (dataFromApp == YELLOW_15_SEC_STR) {
      //Serial.println("turn on led yellow for 15 secs");
      LED_YELLOW_TIMER = 15000;
      toggleYellowLED(1);
    }
    else if (dataFromApp == YELLOW_ALWAYS_ON_STR) {
      //Serial.println("turn on led yellow always");
      LED_YELLOW_TIMER = 0;
      toggleYellowLED(1);
    }
    else if (dataFromApp == YELLOW_OFF_STR) {
      LED_YELLOW_TIMER = 0;
      toggleYellowLED(0);
    }
    else if (dataFromApp == SYNC_STR) {
     //Serial.println("sync call from android");
     delay(5);
     getLEDStatus();
     delay(5);
    }
  }
}
