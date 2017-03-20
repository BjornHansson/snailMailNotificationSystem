#include <Wire.h>
#include <UnoWiFiDevEd.h>

/**
  Setup:
  Ultrasonic sensor to detect delivered snail mails to a mailbox. Code inspired from: https://codebender.cc/sketch:356078#HC-SR04%20Ultrasonic%20Sensor%20Example.ino
  Photoresistor to detect when the user is emptying the mailbox.
  Simple web server which exposes the number of mails.
  Outputs to a serial monitor for debug purposes.
*/

// Pins
const unsigned int PHOTORESISTOR_PIN = 0;
const unsigned int ULTRASONIC_ECHO_PIN = 9;
const unsigned int ULTRASONIC_TRIG_PIN = 10;
// Delay time value (currently 1s)
const unsigned int TIME = 1000;
// Value to recognize if mail is delivered (use higher value if the mailbox is larger)
const unsigned int ULTRASONIC_MAX = 1600;
// Value to recognize if user is emptying the mailbox (use higher value if it is bright in the mailbox)
const unsigned int PHOTORESISTOR_MAX = 400;
// Counter for delivered mails
unsigned int deliveredMails = 0;

// Setup code, to run once
void setup() {
  // The trigger pin will tell the sensor to range find
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  // Use the serial monitor to view the sensors output
  Serial.begin(9600);
  // Initializes the WiFi library's network settings and provides the current status.
  Wifi.begin();
  Wifi.println("WebServer is up");
}

// Main code, to run repeatedly
void loop() {
  // Check if a new mail is delivered
  checkIfNewMail();

  // Check if the mailbox is open
  while (analogRead(PHOTORESISTOR_PIN) > PHOTORESISTOR_MAX) {
    mailboxIsOpen();
  }

  // Process client communication via wifi
  while (Wifi.available()) {
    process(Wifi);
  }
}

void checkIfNewMail() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulseWidth;
  float cm;
  // Hold the trigger pin high for at least 10 us
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  // Wait for pulse on echo pin
  while ( digitalRead(ULTRASONIC_ECHO_PIN) == 0 );
  // Measure how long the echo pin was held high (pulse width). Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while (digitalRead(ULTRASONIC_ECHO_PIN) == 1);
  t2 = micros();
  pulseWidth = t2 - t1;
  // Calculate distance in centimeters
  cm = pulseWidth / 58.0;
  // Check if it should recognize a hit
  if (pulseWidth < ULTRASONIC_MAX) {
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");
    newMail();
  }
  // Wait before next measurement
  delay(TIME / 2);
}

void newMail() {
  deliveredMails++;
  Serial.println("New mail");
  Serial.print("Total delivered mails: ");
  Serial.println(deliveredMails);
  // Extra wait when a new mail is detected, because we do not want to register the same several times
  delay(TIME);
}

void mailboxIsOpen() {
  Serial.println("Mailbox is open");
  Serial.print("Value from PHOTORESISTOR_PIN: ");
  Serial.println(analogRead(PHOTORESISTOR_PIN));
  deliveredMails = 0;
  // Wait until mailbox is closed
  delay(TIME);
}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');
  if (command == "digital") {
    digitalCommand(client);
  }
}

void digitalCommand(WifiData client) {
  // Send feedback to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
  client.print("{\"mails\":");
  client.print(deliveredMails);
  client.println("}");
  client.print(EOL); //char terminator
}
