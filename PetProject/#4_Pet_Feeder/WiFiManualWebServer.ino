#include <ESP.h>
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "SK_WiFiGIGA8D89"
#define STAPSK  "1608014530"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

int client_open_cnt = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(8712); // Port 8712

void setup() {
  Serial.begin(9600);

  // prepare GPIO
  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

/* Set static ip */
  IPAddress ip(192,168,35,120);
  IPAddress gateway(192,168,35,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(ip, gateway, subnet);

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
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.println(" ");
}

void loop() {
/* ## Normal loop ## */
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    delay(500);
    digitalWrite(D0, 1);
    delay(500);
    digitalWrite(D0, 0);
    return;
  }

/* ## Enter Client Process ## */
  Serial.println(F("## Enter new client"));
  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String rxData_from_client = client.readStringUntil('\r');
  Serial.print(F("Request: "));
  Serial.println(rxData_from_client);
  client.flush();

  // Match the request
  int txData_to_client;
  
  if (rxData_from_client.indexOf(F("CMD=RUN")) != -1)
  {
    txData_to_client = 1;
    digitalWrite(D1, 1);
    digitalWrite(D2, 1);
    client_open_cnt++;
  }
  else
  {
    txData_to_client = 0;
    Serial.println(F("**Invalid request"));
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
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n"));
  client.print(F("## PET AUTOMATIC FEEDER ##"));
  client.print(F("<br><br>"));
  client.print(F("CURRENT STATUS : "));
  client.print((txData_to_client) ? F("RUNNING!!") : F("Not service"));
  client.print((client_open_cnt >= 3) ? F("\n\rDeepSleep!!") : F(""));

  client.println(F("<br><br>"));
  client.println(F("<a href=\"CMD=RUN\"\"><button>PLAY</button></a><br/><br/>"));
  client.println(F("</html>"));


  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client ##"));
  Serial.println(" ");

/* Default Port Setting */
  delay(650);
  digitalWrite(D1, 0);
  digitalWrite(D2, 0);
/* Check client open count */
  if(client_open_cnt > 3)
  {
    digitalWrite(D0, 1);
    Serial.println(" ");
    Serial.println("## DeepSleep Mode ##");
    delay(20000);
    ESP.deepSleep(18000e6);  // DeepSleep 3Hour
  }
}
