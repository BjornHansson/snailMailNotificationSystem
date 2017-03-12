#include <Wire.h>
#include <UnoWiFiDevEd.h>

void setup() {
  pinMode(13, OUTPUT);
  Wifi.begin();
  Wifi.println("WebServer is up");
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
  int clearMails = client.parseInt();
  int nrOfMails = 1;
  if(clearMails == 1) {
    nrOfMails = 0;
  }
  // Send feedback to client
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
  client.print("{\"mails\":");
  client.print(nrOfMails);
  client.println("}");
  client.print(EOL); //char terminator
}
