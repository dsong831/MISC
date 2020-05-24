/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "SK_WiFiGIGA8D89"
#define STAPSK  "1608014530"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  // prepare GPIO
  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  int val;
  if (req.indexOf(F("/gpio/0")) != -1)
  {
    val = 0;
    digitalWrite(D0, 0);
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/1")) != -1)
  {
    val = 1;
    digitalWrite(D0, 1);
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/2")) != -1)
  {
    val = 2;
    digitalWrite(D0, 0);
    digitalWrite(D1, 1);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/3")) != -1)
  {
    val = 3;
    digitalWrite(D0, 1);
    digitalWrite(D1, 1);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/4")) != -1)
  {
    val = 4;
    digitalWrite(D0, 0);
    digitalWrite(D1, 0);
    digitalWrite(D2, 1);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/5")) != -1)
  {
    val = 5;
    digitalWrite(D0, 1);
    digitalWrite(D1, 0);
    digitalWrite(D2, 1);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/6")) != -1)
  {
    val = 6;
    digitalWrite(D0, 0);
    digitalWrite(D1, 1);
    digitalWrite(D2, 1);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/7")) != -1)
  {
    val = 7;
    digitalWrite(D0, 1);
    digitalWrite(D1, 1);
    digitalWrite(D2, 1);
    digitalWrite(D3, 0);
  }
  else if (req.indexOf(F("/gpio/8")) != -1)
  {
    val = 8;
    digitalWrite(D0, 0);
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 1);
  }
  else if (req.indexOf(F("/gpio/9")) != -1)
  {
    val = 9;
    digitalWrite(D0, 1);
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 1);
  }
  else
  {
    Serial.println(F("invalid request"));
    val = digitalRead(LED_BUILTIN);
  }

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nSelected number is "));
  client.print((val) ? F("valid") : F("invalid"));

  client.print(F("<br><br>Number 0 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>send</a> to client."));

  client.print(F("<br><br>Number 1 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>send</a> to client."));
 
  client.print(F("<br><br>Number 2 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/2'>send</a> to client."));

  client.print(F("<br><br>Number 3 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/3'>send</a> to client."));

  client.print(F("<br><br>Number 4 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/4'>send</a> to client."));

  client.print(F("<br><br>Number 5 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/5'>send</a> to client."));

  client.print(F("<br><br>Number 6 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/6'>send</a> to client."));

  client.print(F("<br><br>Number 7 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/7'>send</a> to client."));

  client.print(F("<br><br>Number 8 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/8'>send</a> to client."));

  client.print(F("<br><br>Number 9 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/9'>send</a> to client."));

  client.print(F("<br><br>Thank you ^^</html>"));


  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
