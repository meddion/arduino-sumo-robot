const int BIT_RATE = 9600;
const int DEFAULT_SPEED = 50;
const int OBSERVABLE_BOTTOM_BOUNDARY = 400;
const int OBSERVABLE_TOP_BOUNDARY = 1000;
const int TESTING_DELAY = 500; // in ms
const int ROTATE_ANGLE_MULTIPLIER = 1; // change it!!

enum LaserSensorCases {BOTH_DIM, BOTH_BRIGHT, LEFT_DIM, RIGHT_DIM};
enum MoveFunctions {NONE, STRAIGHT, BACK, LEFT, RIGHT, HALF_LEFT, HALF_RIGHT};
MoveFunctions lastUsedMoveFunc = NONE;

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

  rotateBy(180);
}

void stopMotors()
{
  analogWrite(leftMotor.pinSpeed, 0);
  analogWrite(rightMotor.pinSpeed, 0);
}

void moveStraight(int speed = DEFAULT_SPEED)
{
  if (lastUsedMoveFunc != STRAIGHT) stopMotors();
  analogWrite(leftMotor.pinSpeed, speed);
  digitalWrite(leftMotor.pinDirection, HIGH);
  analogWrite(rightMotor.pinSpeed, speed);
  digitalWrite(rightMotor.pinDirection, HIGH);
  lastUsedMoveFunc = STRAIGHT;
  makeNextMoveDecision();
}

void moveBack(int speed = DEFAULT_SPEED)
{
  if (lastUsedMoveFunc != BACK) stopMotors();
  analogWrite(leftMotor.pinSpeed, speed);
  digitalWrite(leftMotor.pinDirection, LOW);
  analogWrite(rightMotor.pinSpeed, speed);
  digitalWrite(rightMotor.pinDirection, LOW);
  lastUsedMoveFunc = BACK;
  makeNextMoveDecision();
}

void moveLeft(int speed = DEFAULT_SPEED)
{
  if (lastUsedMoveFunc != LEFT) stopMotors();
  analogWrite(leftMotor.pinSpeed, speed);
  digitalWrite(leftMotor.pinDirection, LOW);
  analogWrite(rightMotor.pinSpeed, speed);
  digitalWrite(rightMotor.pinDirection, HIGH);
  lastUsedMoveFunc = LEFT;
  makeNextMoveDecision();
}

void moveRight(int speed = DEFAULT_SPEED)
{
  if (lastUsedMoveFunc != RIGHT) stopMotors();
  analogWrite(leftMotor.pinSpeed, speed);
  digitalWrite(leftMotor.pinDirection, HIGH);
  analogWrite(rightMotor.pinSpeed, speed);
  digitalWrite(rightMotor.pinDirection, LOW);
  lastUsedMoveFunc = RIGHT;
  makeNextMoveDecision();
}

void moveHalfLeft(int speed = DEFAULT_SPEED)
{
  if (lastUsedMoveFunc != HALF_LEFT) stopMotors();
  analogWrite(rightMotor.pinSpeed, speed);
  digitalWrite(rightMotor.pinDirection, HIGH);
  lastUsedMoveFunc = HALF_LEFT;
  makeNextMoveDecision();
}

void moveHalfRight(int speed = DEFAULT_SPEED)
{
  if (lastUsedMoveFunc != HALF_RIGHT) stopMotors();
  analogWrite(leftMotor.pinSpeed, speed);
  digitalWrite(leftMotor.pinDirection, HIGH);
  lastUsedMoveFunc = HALF_RIGHT;
  makeNextMoveDecision();
}

// clockwise (right wheel - still, left - moving)
void rotateBy(int angle) 
{
  int speed = angle * ROTATE_ANGLE_MULTIPLIER;
  analogWrite(leftMotor.pinSpeed, speed);
  digitalWrite(leftMotor.pinDirection, HIGH);
}

bool doesLeftDistSensorSee() {
  return leftDistSensor.value >= OBSERVABLE_BOTTOM_BOUNDARY && leftDistSensor.value <= OBSERVABLE_TOP_BOUNDARY;
}

bool doesRightDistSensorSee() {
  return rightDistSensor.value >= OBSERVABLE_BOTTOM_BOUNDARY && rightDistSensor.value <= OBSERVABLE_TOP_BOUNDARY;
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
      moveBack();
      rotateBy(180); 
      break;
    case LEFT_DIM: // move back and right
      moveBack();
      moveHalfRight();
      break;
    case RIGHT_DIM: // move back and left
      moveBack();
      moveHalfLeft();
     break;
  }
  
  if (doesLeftDistSensorSee() && !doesRightDistSensorSee()) moveHalfLeft();
  else if (doesLeftDistSensorSee() && !doesRightDistSensorSee()) moveHalfRight();
  
  while (doesLeftDistSensorSee() && doesRightDistSensorSee()) {
    do moveStraight(200); while (abs(leftDistSensor.value - rightDistSensor.value) <= 5);

    if (leftDistSensor.value < rightDistSensor.value) moveLeft();
    else if (leftDistSensor.value > rightDistSensor.value) moveRight();
  }
}

void loop()
{
  moveStraight();
  delay(TESTING_DELAY);
}
