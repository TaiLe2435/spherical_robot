const int BIN_1 = 10;
const int BIN_2 = 9;
const int AIN_2 = 8;
const int AIN_1 = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(BIN_1, OUTPUT);
  pinMode(BIN_2, OUTPUT);
  pinMode(AIN_1, OUTPUT);
  pinMode(AIN_2, OUTPUT);
  
  digitalWrite(BIN_1, LOW);
  digitalWrite(BIN_2, LOW);
  digitalWrite(AIN_1, LOW);
  digitalWrite(AIN_2, LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // forward
  analogWrite(BIN_1, 80);
  digitalWrite(BIN_2, LOW);
  analogWrite(AIN_2, 80);
  digitalWrite(AIN_1, LOW);
  delay(1000);

  // backwards
//  analogWrite(BIN_2, 120);
//  digitalWrite(BIN_1, LOW);
//  analogWrite(AIN_1, 120);
//  digitalWrite(AIN_2, LOW);
//  delay(1000);
}
