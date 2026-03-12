#include <Wire.h>
#include "Adafruit_AS726x.h"

Adafruit_AS726x sensor;

// Ultrasonic sensor pins
int trigLeft = 9;
int echoLeft = 8;
int trigCenter = 7;
int echoCenter = 6;
int trigRight = 5;
int echoRight = 4;

// Motor pins
int enLeft = 3;
int inLeft1 = 2;
int inLeft2 = 12;
int enRight = 11;
int inRight1 = 13;
int inRight2 = A3;

// Relay pins
int relayPin = 10;

int normal_speed = 50;
int current_speed = 50;


void setup()
{
    Serial.begin(9600);

    // ------ Motor driver setup ------ 
    pinMode(enLeft, OUTPUT);
    pinMode(inLeft1, OUTPUT);
    pinMode(inLeft2, OUTPUT);
    pinMode(enRight, OUTPUT);
    pinMode(inRight1, OUTPUT);
    pinMode(inRight2, OUTPUT);
    
    digitalWrite(inLeft1, LOW);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, LOW);
    digitalWrite(inRight2, LOW);

    analogWrite(enLeft, 255);
    analogWrite(enRight, 255);


    // ------ Color sensor setup ------ 
    if (!sensor.begin()) {
        Serial.println("Sensor not found");
        while (1);
    }

    sensor.setDrvCurrent(1);   // 25 mA LED
    sensor.drvOn();            // turn illumination LED on
    sensor.setIntegrationTime(50);
    Serial.println("AS7262 Ready");

    pinMode(trigLeft, OUTPUT);
    pinMode(echoLeft, INPUT);

    pinMode(trigCenter, OUTPUT);
    pinMode(echoCenter, INPUT);

    pinMode(trigRight, OUTPUT);
    pinMode(echoRight, INPUT);


    // ------ Relay pin setup ------ 
    pinMode(relayPin, OUTPUT);
}


// ---------------------------------------------
// Ultrasonic sensor module 
// ---------------------------------------------
// Read one ultrasonic sensor
long readUltrasonic(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    long distance = duration * 0.0343 / 2.0;
    return distance;
}

// Modular update block for all 3 sensors
// returns long array of distances [left, center, right]
void getUltrasonicReadings(long readings[]) 
{
    readings[0] = readUltrasonic(trigLeft, echoLeft);
    delay(60);

    readings[1] = readUltrasonic(trigCenter, echoCenter);
    delay(60);

    readings[2] = readUltrasonic(trigRight, echoRight);
    delay(60);
}



// helper functions for movement
void moveForward(){
    analogWrite(enLeft, currentSpeed);
    analogWrite(enRight, currentSpeed);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}


void moveLeft(){
    analogWrite(enLeft, currentSpeed - 15);
    analogWrite(enRight, currentSpeed + 15);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}


void moveRight(){
    analogWrite(enLeft, currentSpeed + 15);
    analogWrite(enRight, currentSpeed - 15);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void moveHardLeft(){
    analogWrite(enLeft, currentSpeed - 30);
    analogWrite(enRight, currentSpeed + 30);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}


void moveHardRight(){
    analogWrite(enLeft, currentSpeed + 30);
    analogWrite(enRight, currentSpeed - 30);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void stopMotors(){
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, LOW);
    digitalWrite(inLeft2, LOW);
    digitalWrite(inRight1, LOW);
    digitalWrite(inRight2, LOW);
}


void moveRobot(long readings[]){
    // placeholder threshold values
    int threshold_left = 30;
    int threshold_center = 30;
    int threshold_right = 30;

    // left, center, right is 1 if there is obstacle in that dirction; otherwise 0
    int left = 0;
    int center = 0;
    int right = 0;

    if (readings[0] < threshold_left){
        left = 1;
    }

    if (readings[1] < threshold_center){
        center = 1;
    }

    if (readings[2] < threshold_right){
        right = 1;
    }

    int state = (left << 2) | (center << 1) | right;

    // determine direction to turn
    switch(state) {
        case 0: // 000
            moveForward();

        case 1: // 001
            moveLeft();

        case 2: // 010
            if (readings[0] < readings[2]){
                moveRight();
            }
            else {
                moveLeft();
            }

        case 3: // 011
            moveHardLeft();
        
        case 4: // 100
            moveRight();
        
        case 5: // 101
            moveForward();

        case 6: // 110
            moveHardRight();
        
        case 7:
            moveHardRight(); // for now, need to test threshold later to determine

}



// void centerDistance(long readings[], int angle){

// }


// Optional debug print block
void printUltrasonicReadings(long readings[])
{
    Serial.print("Left: ");
    Serial.print(readings[0]);

    Serial.print("Center: ");
    Serial.print(readings[1]);

    Serial.print("Right: ");
    Serial.print(readings[2]);

    Serial.println();
}

// ---------------------------------------------
// colour sensor
// ---------------------------------------------
int detect_red(){
    int RED_DETECTED = 0;

    sensor.startMeasurement();
        while (!sensor.dataReady()) {
        delay(5);
    }

    float violet = sensor.readCalibratedViolet();
    float blue   = sensor.readCalibratedBlue();
    float green  = sensor.readCalibratedGreen();
    float yellow = sensor.readCalibratedYellow();
    float orange = sensor.readCalibratedOrange();
    float red    = sensor.readCalibratedRed();

    // check if red and orange are highest
    if (red >= violet && red >= blue && red >= green && red >= yellow &&
        orange >= violet && orange >= blue && orange >= green && orange >= yellow) {
        RED_DETECTED = 1;
        Serial.println("RED DETECTED");
    }

    Serial.print(RED_DETECTED);
    delay(500);

    return RED_DETECTED;
}



// ---------------------------------------------
// Relay pump module 
// ---------------------------------------------
// activate the pump
void activatePump() {
    digitalWrite(relayPin, LOW);   // pump ON
    Serial.println("activated");
}

// deactivate the pump
void deactivatePump() {
    digitalWrite(relayPin, HIGH); // pump OFF
}





void loop()
{
    long left = readUltrasonic(trigLeft, echoLeft);
    delay(60);

    long center = readUltrasonic(trigCenter, echoCenter);
    delay(60);

    long right = readUltrasonic(trigRight, echoRight);
    delay(60);

    Serial.print("Left: ");
    Serial.print(left);
    Serial.print(" cm | Center: ");
    Serial.print(center);
    Serial.print(" cm | Right: ");
    Serial.print(right);
    Serial.println(" cm");

    delay(500);

    detect_red();

    // activatePump();
    // delay(5000);
    // deactivatePump();
}