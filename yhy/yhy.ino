const int BIT_RATE = 9600;
const int DEFAULT_SPEED = 50;
const int OBSERVABLE_BOTTOM_BOUNDARY = 400;
const int OBSERVABLE_TOP_BOUNDARY = 900;

enum Desicion {BOTH_DIM, BOTH_BRIGHT, FIRST_DIM, SECOND_DIM};

struct {
  int pinName;
  int value;
} 
leftDistSensor, rightDistSensor, leftLaserSensor, rightLaserSensor;

struct {
  int pinDirection;
  int pinSpeed;
} 
leftMotor, rightMotor;

void setup()
{
  Serial.begin(BIT_RATE);
  leftMotor.pinDirection = 9;
  leftMotor.pinSpeed = 3;
  rightMotor.pinDirection = 13;
  rightMotor.pinSpeed = 10;

  leftDistSensor.pinName = 4;
  rightDistSensor.pinName = 5;
  leftLaserSensor.pinName = 6;
  rightLaserSensor.pinName = 7;

  pinMode(leftMotor.pinDirection, OUTPUT);
  pinMode(leftMotor.pinSpeed, OUTPUT);
  pinMode(rightMotor.pinDirection, OUTPUT);
  pinMode(rightMotor.pinSpeed, OUTPUT);
  pinMode(leftLaserSensor.pinName, INPUT);
  pinMode(rightLaserSensor.pinName, INPUT);
}

void moveStraight(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(leftMotor.pinDirection, HIGH);
    analogWrite(rightMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(rightMotor.pinDirection, HIGH);
    delay(1500);
  }
}

void moveBack(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(leftMotor.pinDirection, LOW);
    analogWrite(rightMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(rightMotor.pinDirection, LOW);
    delay(1500);
  }
}

void moveLeft(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(leftMotor.pinDirection, LOW);
    analogWrite(rightMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(rightMotor.pinDirection, HIGH);
    delay(1500);
  }
}

void moveRight(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(leftMotor.pinDirection, HIGH);
    analogWrite(rightMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(rightMotor.pinDirection, LOW);
    delay(1500);
  }
}

void moveHalfLeft(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(rightMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(rightMotor.pinDirection, HIGH);
    delay(1500);
  }
}

void moveHalfRight(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, DEFAULT_SPEED);
    digitalWrite(leftMotor.pinDirection, HIGH);
    delay(1500);
  }
}

bool doesLeftDistSensorSee() {
  return leftDistSensor.value >= OBSERVABLE_BOTTOM_BOUNDARY && leftDistSensor.value < OBSERVABLE_TOP_BOUNDARY;
}

bool doesRightDistSensorSee() {
  return rightLaserSensor.value >= OBSERVABLE_BOTTOM_BOUNDARY && rightLaserSensor.value < OBSERVABLE_TOP_BOUNDARY;
}

/*void testDistSensors()
{
  Serial.print("Left distance: ");
  Serial.println(leftDistSensor.value);
  Serial.print("Right distance: ");
  Serial.println(rightDistSensor.value);
}

void testLaserSensors()
{
  Serial.print("Left laser: ");
  Serial.println(leftLaserSensor.value);
  Serial.print("Right laser: ");
  Serial.println(rightLaserSensor.value);
}*/

int compareLasersInput()
{
  if (leftLaserSensor.value == 1 && rightLaserSensor.value == 1) return BOTH_BRIGHT;
  if (leftLaserSensor.value == 1 && rightLaserSensor.value == 0) return SECOND_DIM;
  if (leftLaserSensor.value == 0 && rightLaserSensor.value == 1) return FIRST_DIM;

  return BOTH_DIM; // if both laser sensors output 0
}

void lookForward()
{
  moveStraight();
  moveLeft();
  moveStraight();
  moveRight(2);
  moveStraight();
  moveLeft();
}

void lookBackward()
{
  moveRight(2);
  moveStraight();
  moveLeft();
  moveStraight();
  moveRight(2);
  moveStraight();
  moveLeft();
}

void loop()
{
  leftLaserSensor.value = digitalRead(leftLaserSensor.pinName);
  rightLaserSensor.value = digitalRead(rightDistSensor.pinName);
  leftDistSensor.value = analogRead(leftDistSensor.pinName);
  rightDistSensor.value = analogRead(rightDistSensor.pinName);

  do
  {
    lookForward();
    lookBackward();
  }
  while (!doesLeftDistSensorSee() && !doesRightDistSensorSee() && compareLasersInput() == BOTH_BRIGHT);

 if (compareLasersInput() == FIRST_DIM) {
    moveBack();
    moveRight();
    moveStraight();
  } else if (compareLasersInput() == SECOND_DIM) {
    moveBack();
    moveLeft();
    moveStraight();
  }
  // turning left or right
  if (doesLeftDistSensorSee() && !doesRightDistSensorSee()) {
    moveHalfLeft();
  } else if (doesLeftDistSensorSee() && !doesRightDistSensorSee()) {
    moveHalfRight();
  }

  while (doesLeftDistSensorSee() && doesRightDistSensorSee())
  {
    do
    {
      moveStraight();
    }
    while (leftDistSensor.value == rightDistSensor.value);

    if (leftDistSensor.value < rightDistSensor.value)
      moveLeft();
    else if (leftDistSensor.value > rightDistSensor.value)
      moveRight();
  }
}