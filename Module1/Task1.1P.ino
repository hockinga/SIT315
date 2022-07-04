// C++ code
//
void setup()
{
  // Log output
  Serial.begin(9600);

  // Setup pins
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
}

void loop()
{
  int motion = digitalRead(3);

  // Detected motion
  if (motion == HIGH)
  {
    Serial.println("Detected motion");

    // Activate LED for 0.5 seconds
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
  }
}
