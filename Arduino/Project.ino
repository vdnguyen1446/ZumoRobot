#include <Wire.h>
#include <ZumoShield.h>

#define LED 13

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1500 // microseconds

// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
unsigned long lastSensorCheck = 0;
const unsigned long checkInterval= 10;

void waitForButton(){
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
}


void setup()
{
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);

  pinMode(LED, HIGH);
  Serial.begin(9600); // open the serial port at 9600 bps:
  sensors.init();

  waitForButton();
}




void loop()
{
  unsigned long currentMillis = millis();

  if(currentMillis - lastSensorCheck >= checkInterval){
    lastSensorCheck = currentMillis;
    sensors.read(sensor_values);


    if (sensor_values[0] > QTR_THRESHOLD)
    {
      // if leftmost sensor detects line, reverse and turn to the right
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
    else if (sensor_values[5] > QTR_THRESHOLD)
    {
      // if rightmost sensor detects line, reverse and turn to the left
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
  }
  
  else
  {
    //go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(200);
    //go back
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(200);
    //turn left
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(200);
    //turn right
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(200);

    
  }
}
