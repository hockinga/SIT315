const int LED_PIN = 2;
const int PIR_PIN = 3;

void setup()
{
  // Log output
  Serial.begin(9600);

  // Setup pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
}

void loop()
{
  int motion = digitalRead(PIR_PIN);

  // Detected motion
  if (motion == HIGH)
  {
    Serial.println("Detected motion");

    // Activate LED for 0.5 seconds
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }
}
