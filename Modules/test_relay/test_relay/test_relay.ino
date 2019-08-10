
int r1 = D7;
int r2 = D8;

void setup() {
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  
  delay(500);
  
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  delay(1000);
}
