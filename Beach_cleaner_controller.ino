#include <RCSwitch.h>

RCSwitch Controller;
int joystickXPin = A2;
int joystickYPin = A1;
int buttonPin = 4;
int switchPin = 3;

int ButtonState = HIGH;

bool state = true;
int mode = 10;

int pickupCommand = 5;

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
        Controller.send(5, 4);
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
        Controller.send(10, 4);
        delay(5);
        i++;
        }
    }
    else if (direction != -1) {
        Controller.send(direction, 4);
        Serial.println(direction);
    } else {
        // Joystick is not touched (right in the middle)
        Controller.send(9, 4);
    }
}

int getJoystickDirection(int x, int y) {
    int xThreshold = 512;
    int yThreshold = 512;

    if (y < yThreshold - 50) {
        return 3;
    } else if (y > yThreshold + 50) {
        return 4;
    } else if (x < xThreshold - 50) {
        return 2;
    } else if (x > xThreshold + 100) {
        return 1;
    } else {
        return -1;
    }
}
