const int LED_PIN = 2;
const int PIR_PIN = 3;

void setup()
{
  // Log output
  Serial.begin(9600);

  // Setup pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  // Interrupt on motion detected
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motion, RISING);
}

void loop()
{
  Serial.println("Main loop execution...");
  delay(200);
}

void motion()
{
  Serial.println("Detected motion");

  // Activate LED
  digitalWrite(LED_PIN, HIGH);
}
