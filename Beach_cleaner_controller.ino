#include <RCSwitch.h>

RCSwitch Controller;
int joystickXPin = A2;
int joystickYPin = A1;
int buttonPin = 4;
int switchPin = 3;

int ButtonState = HIGH;

bool state = true;
int changeMode = 40;
int doNothing = 39;
int pickupCommand = 35;

void setup() {
    Controller.enableTransmit(2);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(switchPin, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    ButtonState = digitalRead(buttonPin);
    int switchState = digitalRead(switchPin);

    if (switchState == LOW) {
        Serial.println("The joystick is pressed");
        Controller.send(pickupCommand, 8);
    }

    int joystickXValue = analogRead(joystickXPin);
    int joystickYValue = analogRead(joystickYPin);
    int buttonState = digitalRead(buttonPin);

    int direction = getJoystickDirection(joystickXValue, joystickYValue);

    if (ButtonState == LOW) {
        Serial.println("The button is pressed");
        int i = 0;
        while(i<8)
        {
        Serial.println(i);
        Controller.send(changeMode, 8);
        delay(5);
        i++;
        }
    }
    else if (direction != -1) {
        Controller.send(direction, 8);
        Serial.println(direction);
    } else {
        // Joystick is not touched (right in the middle)
        Controller.send(doNothing, 8);
    }
}

int getJoystickDirection(int x, int y) {
    int xThreshold = 512;
    int yThreshold = 512;

    if (y < yThreshold - 50) {
        return 33;
    } else if (y > yThreshold + 50) {
        return 34;
    } else if (x < xThreshold - 50) {
        return 32;
    } else if (x > xThreshold + 100) {
        return 31;
    } else {
        return -1;
    }
}
