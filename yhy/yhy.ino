const int BIT_RATE = 9600;
const int DEFAULT_SPEED = 50;
const int OBSERVABLE_BOTTOM_BOUNDARY = 400;
const int OBSERVABLE_TOP_BOUNDARY = 900;
const int TESTING_DELAY = 500; // in ms

enum LaserSensorCases {BOTH_DIM, BOTH_BRIGHT, LEFT_DIM, RIGHT_DIM};
enum LastUsedMoveFunc {NONE, STRAIGHT, BACK, LEFT, RIGHT, HALF_LEFT, HALF_RIGHT} lastUsedMoveFunc;
lastUsedMoveFunc = NONE;

int lastUsed;

struct {
  int pinName;
  int value;
} leftDistSensor, rightDistSensor, leftLaserSensor, rightLaserSensor;

struct {
  int pinDirection;
  int pinSpeed;
} leftMotor, rightMotor;

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

void stopMotors()
{
  analogWrite(leftMotor.pinSpeed, 0);
  analogWrite(rightMotor.pinSpeed, 0);
}

void moveStraight(int times = 1, int speed = 0)
{
  if (!speed) speed = DEFAULT_SPEED;
  if (lastUsedMoveFunc != STRAIGHT) stopMotors();
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, speed);
    digitalWrite(leftMotor.pinDirection, HIGH);
    analogWrite(rightMotor.pinSpeed, speed);
    digitalWrite(rightMotor.pinDirection, HIGH);
    lastUsedMoveFunc = STRAIGHT;
    makeNextMoveDecision();
  }
}

void moveBack(int times = 1, int speed = 0)
{
  if (!speed) speed = DEFAULT_SPEED;
  if (lastUsedMoveFunc != BACK) stopMotors();
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, speed);
    digitalWrite(leftMotor.pinDirection, LOW);
    analogWrite(rightMotor.pinSpeed, speed);
    digitalWrite(rightMotor.pinDirection, LOW);
    lastUsedMoveFunc = BACK;
    makeNextMoveDecision();
  }
}

void moveLeft(int times = 1, int speed = 0)
{
  if (!speed) speed = DEFAULT_SPEED;
  if (lastUsedMoveFunc != LEFT) stopMotors();
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, speed);
    digitalWrite(leftMotor.pinDirection, LOW);
    analogWrite(rightMotor.pinSpeed, speed);
    digitalWrite(rightMotor.pinDirection, HIGH);
    lastUsedMoveFunc = LEFT;
    makeNextMoveDecision();
  }
}

void moveRight(int times = 1, int speed = 0)
{
  if (!speed) speed = DEFAULT_SPEED;
  if (lastUsedMoveFunc != RIGHT) stopMotors();
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, speed);
    digitalWrite(leftMotor.pinDirection, HIGH);
    analogWrite(rightMotor.pinSpeed, speed);
    digitalWrite(rightMotor.pinDirection, LOW);
    lastUsedMoveFunc = RIGHT;
    makeNextMoveDecision();
  }
}

void moveHalfLeft(int times = 1, int speed = 0)
{
  if (!speed) speed = DEFAULT_SPEED;
  if (lastUsedMoveFunc != HALF_LEFT) stopMotors();
  for (int i = 0; i < times; i++) {
    analogWrite(rightMotor.pinSpeed, speed);
    digitalWrite(rightMotor.pinDirection, HIGH);
    lastUsedMoveFunc = HALF_LEFT;
    makeNextMoveDecision();
  }
}

void moveHalfRight(int times = 1, int speed = 0)
{
  if (!speed) speed = DEFAULT_SPEED;
  if (lastUsedMoveFunc != HALF_RIGHT) stopMotors();
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, speed);
    digitalWrite(leftMotor.pinDirection, HIGH);
    lastUsedMoveFunc = HALF_RIGHT;
    makeNextMoveDecision();
  }
}

bool doesLeftDistSensorSee() {
  return leftDistSensor.value >= OBSERVABLE_BOTTOM_BOUNDARY && leftDistSensor.value < OBSERVABLE_TOP_BOUNDARY;
}

bool doesRightDistSensorSee() {
  return rightDistSensor.value >= OBSERVABLE_BOTTOM_BOUNDARY && rightDistSensor.value < OBSERVABLE_TOP_BOUNDARY;
}

int getLaserSensorCase()
{
  if (leftLaserSensor.value == 1 && rightLaserSensor.value == 1) return BOTH_BRIGHT;
  if (leftLaserSensor.value == 1 && rightLaserSensor.value == 0) return RIGHT_DIM;
  if (leftLaserSensor.value == 0 && rightLaserSensor.value == 1) return LEFT_DIM;
  return BOTH_DIM; // if both laser sensors output 0
}

void getSensorsInput() 
{
  leftLaserSensor.value = digitalRead(leftLaserSensor.pinName);
  rightLaserSensor.value = digitalRead(rightDistSensor.pinName);
  leftDistSensor.value = analogRead(leftDistSensor.pinName);
  rightDistSensor.value = analogRead(rightDistSensor.pinName);
}

void makeNextMoveDecision()
{
  getSensorsInput();

  switch (getLaserSensorCase()) {
    case BOTH_DIM:
      // move somewhere
      break;
    case LEFT_DIM:
      // move somewhere
      break;
    case RIGHT_DIM:
      // move somewhere
     break;
  }
  
  if (doesLeftDistSensorSee() && !doesRightDistSensorSee()) moveHalfLeft();
  else if (doesLeftDistSensorSee() && !doesRightDistSensorSee()) moveHalfRight();
  
  while (doesLeftDistSensorSee() && doesRightDistSensorSee()) {
    do moveStraight(); while (abs(leftDistSensor.value - rightDistSensor.value) <= 5);

    if (leftDistSensor.value < rightDistSensor.value) moveLeft();
    else if (leftDistSensor.value > rightDistSensor.value) moveRight();
  }
}

void loop()
{
  lookForward();
  lookBackward();
}
