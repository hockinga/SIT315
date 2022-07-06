const int PIR1_PIN = 2;
const int PIR2_PIN = 3;
const int RED_PIN = 4;
const int GRN_PIN = 5;

void setup()
{
  // Log output
  Serial.begin(9600);

  // Setup pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GRN_PIN, OUTPUT);
  pinMode(PIR1_PIN, INPUT);
  pinMode(PIR2_PIN, INPUT);

  // Setup interrupts
  attachInterrupt(digitalPinToInterrupt(PIR1_PIN), pir1_motion, HIGH);
  attachInterrupt(digitalPinToInterrupt(PIR2_PIN), pir2_motion, HIGH);
}

void loop()
{
  delay(100);
}

void pir1_motion()
{
  // Activate red LED on PIR1 motion
  Serial.println("PIR1 Activated (Set: Red = On, Grn = Off)");
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GRN_PIN, LOW);
}

void pir2_motion()
{
  // Activate green LED on PIR2 motion
  Serial.println("PIR2 Activated (Set: Red = Off, Grn = On)");
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GRN_PIN, HIGH);
}
