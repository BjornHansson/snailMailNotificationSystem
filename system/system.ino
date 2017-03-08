/**
  Ultrasonic sensor code inspired from: https://codebender.cc/sketch:356078#HC-SR04%20Ultrasonic%20Sensor%20Example.ino

  Ultrasonic sensor to detect delivered snail mails.
  Outputs to a serial monitor.
*/
// Pins
const int TRIG_PIN = 10;
const int ECHO_PIN = 9;
// Distance to recognize if mail is delivered
const unsigned int MAIL_DIST = 500;
// Counter for delivered mails
int deliveredMails = 0;

void setup() {
  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void loop() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulseWidth;
  float cm;
  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );
  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulseWidth = t2 - t1;
  // Calculate distance in centimeters
  cm = pulseWidth / 58.0;
  // Print out results
  if (pulseWidth < MAIL_DIST) {
    Serial.println("New mail");
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");
    deliveredMails++;
    Serial.print("Total delivered mails: ");
    Serial.println(deliveredMails);
    // Extra wait when a new mail is detected, because we do not want to register the same several times
    delay(1000);
  }
  // Wait at least 1s before next measurement
  delay(1000);
}
