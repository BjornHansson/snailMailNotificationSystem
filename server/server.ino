#include <Wire.h>
#include <UnoWiFiDevEd.h>

void setup() {
  pinMode(13, OUTPUT);
  Wifi.begin();
  Wifi.println("WebServer Server is up");
}

void loop() {
  while (Wifi.available()) {
    process(Wifi);
  }
  delay(50);
}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');
  if (command == "digital") {
    digitalCommand(client);
  }
}

void digitalCommand(WifiData client) {
  int pin;
  // Read pin number
  pin = client.parseInt();
  // Send feedback to client
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
  client.println("{\"mails\":1}");
  client.print(EOL); //char terminator
}