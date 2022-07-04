// C++ code
//
void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
}

void loop()
{
  int motion = digitalRead(3);
  if (motion == HIGH)
  {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
  }
}
