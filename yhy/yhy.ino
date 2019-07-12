const int MAX_VELOCITY = 200;
const int BIT_RATE = 9600;

int hwleft, hwright;
int distanceLeft, distanceRight;

struct {
  short pinName;
  short value;
} 
distSensor1, distSensor2, laserSensor1, laserSensor2;

struct {
  short pinDirection;
  short pinSpeed;
} 
leftMotor, rightMotor;

void setup()
{
  Serial.begin(BIT_RATE);
  leftMotor.pinDirection = 9;
  leftMotor.pinSpeed = 3;
  rightMotor.pinDirection = 13;
  rightMotor.pinSpeed = 10;

  distSensor1.pinName = 4;
  distSensor2.pinName = 5;
  laserSensor1.pinName = 6;
  laserSensor2.pinName = 7;

  pinMode(leftMotor.pinDirection, OUTPUT);
  pinMode(leftMotor.pinSpeed, OUTPUT);
  pinMode(rightMotor.pinDirection, OUTPUT);
  pinMode(rightMotor.pinSpeed, OUTPUT);
  pinMode(laserSensor1.pinName, INPUT);
  pinMode(laserSensor2.pinName, INPUT);
}

void moveStraight(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, 50);
    digitalWrite(leftMotor.pinDirection, HIGH);
    analogWrite(rightMotor.pinSpeed, 50);
    digitalWrite(rightMotor.pinDirection, HIGH);
    delay(500);
  }
}

void moveBack(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, 50);
    digitalWrite(leftMotor.pinDirection, LOW);
    analogWrite(rightMotor.pinSpeed, 50);
    digitalWrite(rightMotor.pinDirection, LOW);
    delay(500);
  }
}

void moveLeft(int times = 1)
{
  for (int i = 0; i < times; i++) {

    analogWrite(leftMotor.pinSpeed, 50);
    digitalWrite(leftMotor.pinDirection, LOW);
    analogWrite(rightMotor.pinSpeed, 50);
    digitalWrite(rightMotor.pinDirection, HIGH);
    delay(500);
  }
}

void moveRight(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, 50);
    digitalWrite(leftMotor.pinDirection, HIGH);
    analogWrite(rightMotor.pinSpeed, 50);
    digitalWrite(rightMotor.pinDirection, LOW);
    delay(500);
  }
}
void moveHalfLeft(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(rightMotor.pinSpeed, 50);
    digitalWrite(rightMotor.pinDirection, HIGH);
    delay(500);
  }
}

void moveHalfRight(int times = 1)
{
  for (int i = 0; i < times; i++) {
    analogWrite(leftMotor.pinSpeed, 50);
    digitalWrite(leftMotor.pinDirection, HIGH);
    delay(500);
  }
}

void lookForward()
{
  moveStraight();
  moveLeft();
  moveStraight();
  moveRight();
  moveRight();
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


void testDistSensors()
{
  Serial.print("Left distance: ");
  Serial.println(distSensor1.value);
  Serial.print("Right distance: ");
  Serial.println(distSensor2.value);
}

void testLaserSensors()
{
  Serial.print("Left laser: ");
  Serial.println(laserSensor1.value);
  Serial.print("Right laser: ");
  Serial.println(laserSensor2.value);
}

short makeMoveDecision()
{
  if (laserSensor1.value == 1 && laserSensor2.value == 1) return 0;

  if (laserSensor1.value == 0 && laserSensor2.value == 0) return 1;

  if (laserSensor1.value == 1 && laserSensor2.value == 0) return 2;

  if (laserSensor1.value == 0 && laserSensor2.value == 1) return 3;
}

void loop()
{
  laserSensor1.value = digitalRead(laserSensor1.pinName);
  laserSensor2.value = digitalRead(laserSensor2.pinName);
  distSensor1.value = analogRead(distSensor1.pinName);
  distSensor2.value = analogRead(distSensor2.pinName);

  testLaserSensors();
  
  /*do
   {
   lookForward();
   lookBackward();
   }
   while (distSensor1.value > 850 && distSensor2.value > 850 && makeMoveDecision());
   
   if (makeMoveDecision() == 0) {
   moveBack();
   moveRight(2);
   }
   
   if (makeMoveDecision() == 2) {
   moveBack();
   moveRight();
   moveStraight();
   }
   
   if (makeMoveDecision() == 3) {
   moveBack();
   moveLeft();
   moveStraight();
   }
   
   if (distSensor1.value <= 850 && distSensor2.value > 850) {
   moveHalfLeft();
   } else if (distSensor1.value > 850 && distSensor2.value <= 850) {
   moveHalfRight();
   }
   
   while (distSensor1.value <= 850 && distSensor2.value <= 850)
   {
   do
   {
   moveStraight();
   }
   while (distSensor1.value == distSensor2.value);
   
   if (distSensor1.value < distSensor2.value) {
   moveLeft();
   }
   
   if (distSensor1.value > distSensor2.value) {
   moveRight();
   }
   }*/
}

