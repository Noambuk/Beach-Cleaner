#include <Wire.h>
#include <Pixy2.h>
#include <LiquidCrystal_I2C.h>
#include <RCSwitch.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RCSwitch Robot = RCSwitch();
Pixy2 pixy;

const int leftPin1 = 3;
const int leftPin2 = 4;
const int rightPin1 = 5;
const int rightPin2 = 7;
const int buttonPin = 3;
const int trigPin = 8;
const int echoPin = 6;
bool target = false;

int pos = 90;
Servo right;
Servo left;


bool state = true; // true for manual mode, false for automatic mode

void setup()
{

  right.attach(10);
  left.attach(9);
  right.write(pos);
  left.write(200 - pos);

  lcd.init();
  lcd.backlight();

  lcd.print("YATKA");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Noam and");
  lcd.setCursor(0, 1);
  lcd.print("Benayahu");
  delay(2000);
  lcd.clear();

  Robot.enableReceive(0);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pixy.init();
  pinMode(leftPin1, OUTPUT);
  pinMode(leftPin2, OUTPUT);
  pinMode(rightPin1, OUTPUT);
  pinMode(rightPin2, OUTPUT);
}


void loop()
{

  if (Robot.available())
  {
    int value = Robot.getReceivedValue();
    handleRemoteCommand(value);
    Robot.resetAvailable();
  }

  if (!state)
  {
    // Automatic mode (controlled by camera)
    pixy.ccc.getBlocks();
    if (pixy.ccc.getBlocks() > 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Trash found");
      for (int i = 0; i < pixy.ccc.numBlocks; i++)
      {
        int objectX = pixy.ccc.blocks[i].m_x;
        int objectY = pixy.ccc.blocks[i].m_y;
        int objectHeight = pixy.ccc.blocks[i].m_height;
        int frameWidth = pixy.frameWidth;
        int maxY = 100;
        int centerX = frameWidth / 2;
        int tolerance = 50;


        if (objectX > centerX + tolerance)
          turnRight(50);

        else if (objectX < centerX - tolerance)
          turnLeft(50);

        else
        {
          moveForwards(50);
        }
        if (objectY + objectHeight > 200)
        {
          Serial.println("Object passed Y=200");
          moveForwards(200);
          pickup();
        }
      }
    }
    else
    {
      lcd.clear();
      // No object detected
      turnRight(1000);
      moveForwards(500);
      stopMotors();
      delay(200);
    }
  }
}

void pickup ()
{
  stopMotors();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Picking up...");
  for (int pos = 10; pos <= 210; pos += 1)  // goes from 0 degrees to 180 degrees in steps of 1 degree
  {
    right.write(pos); // tell servo to go to position in variable 'pos'
    left.write(192 - pos);
    delay(5);
  }
  right.write(10);
  left.write(160);
  delay(500);
  lcd.clear();
}


void handleRemoteCommand(int value)
{
  if (value != 39)
  {
    Serial.print("Received value: ");
    Serial.println(value);
  }

  // Check if the received value is the toggle value
  if (value == 40)
  {
    toggleMode();
  }
  else if (state)
  {
    // Process commands only in manual mode
    if (value == 31)
      moveBackwards(0);

    else if (value == 32)
      moveForwards(0);

    else if (value == 33)
      turnLeft(0);

    else if (value == 34)
      turnRight(0);

    else if (value == 35)
      pickup();

    else if (value == 39)
      stopMotors();

    else
    {
      stopMotors();
    }
  }
}



void toggleMode()
{
  state = !state;  // Toggle the mode
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(state ? "Manual" : "Automatic");
  delay(2000);  // Display the mode for a short time
}

void moveForwards(int duration) 
{
  setMotorOutputs(HIGH, LOW, HIGH, LOW);
  delay(duration);
}

void moveBackwards(int duration) 
{
  setMotorOutputs(LOW, HIGH, LOW, HIGH);
  delay(duration);
}

void turnLeft(int duration) {
  setMotorOutputs(LOW, HIGH, HIGH, LOW);
  delay(duration);
}

void turnRight(int duration) 
{
  setMotorOutputs(HIGH, LOW, LOW, HIGH);
  delay(duration);
}

void stopMotors() 
{
  setMotorOutputs(LOW, LOW, LOW, LOW);
}



void setMotorOutputs(int leftDrive1, int leftDrive2, int rightDrive1, int rightDrive2) 
{
  if (leftDrive1 != 0 && leftDrive2 != 0 && rightDrive1 != 0 && rightDrive2 != 0)
  {
    Serial.print("Motor Outputs: ");
    Serial.print(leftDrive1);
    Serial.print(", ");
    Serial.print(leftDrive2);
    Serial.print(", ");
    Serial.print(rightDrive1);
    Serial.print(", ");
    Serial.println(rightDrive2);
  }
  digitalWrite(leftPin1, leftDrive1);
  digitalWrite(leftPin2, leftDrive2);
  digitalWrite(rightPin1, rightDrive1);
  digitalWrite(rightPin2, rightDrive2);
}
