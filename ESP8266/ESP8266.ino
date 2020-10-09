#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

// Define variables if not defined
#ifndef WIFI_SSID
#define WIFI_SSID "MyWifi" // Wifi ssid / name
#define WIFI_PSK  "hello" // Wifi password
#define HOSTNAME "Table Watcher"
#define TRIGGER_PIN 14 // D5 (Ultrasonic sensor)
#define ECHO_PIN 12 // D6 (Ultrasonic sensor)
#define MEASURE_CORRECTION 11 // Added to measure value (+11 cm)
#define LED LED_BUILTIN // Indicator LED
#define DHCP false // if true ignore everything underneath
#define IP 192,168,2,4 // make sure you use comma instead of dot
#define GATEWAY 192,168,2,1
#define SUBNET 255,255,255,0
#endif

// Init web server with port 80
ESP8266WebServer server(80);

// JSON helper method
void send_json(DynamicJsonDocument res, int code = 200){
  String raw_res;
  serializeJson(res, raw_res);
  server.send(200, "text/json", raw_res);
}

// Index route
void index_route() {
  DynamicJsonDocument res(256);
  res["title"] = "Hello World";
  res["text"] = "Please visit the /measure page!";
  send_json(res);
}
// Not found route
void not_found_route() {
  DynamicJsonDocument res(512);
  res["message"] = "Not found!";
  res["uri"] = server.uri();
  res["method"] = (server.method() == HTTP_GET) ? "GET" : "POST";
  res["rssi"] = WiFi.RSSI();
  send_json(res, 404);
}
// Measure route
void measure_route() {
  DynamicJsonDocument res(128);
  res["table_height"] = measure_table_height() + MEASURE_CORRECTION;
  send_json(res);
}
// Measure height using ultrasonic sensor
int measure_table_height(){ // in cm
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.01715;
}

void setup(void) {
  // Enable Serial for testing purposes
  Serial.begin(115200);

  // Setup LED
  pinMode(LED, OUTPUT);

  // Setup ultrasonic sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Reset wifi settings
  if(ESP.getResetReason() != "Power on") {
     WiFi.mode(WIFI_OFF);
  }

  // Initialize wifi
  WiFi.begin(WIFI_SSID, WIFI_PSK);
  WiFi.hostname(HOSTNAME);

  // Setup DHCP if needed
  if(!DHCP){
    IPAddress ip(IP);
    IPAddress gateway(GATEWAY);
    IPAddress subnet(SUBNET);
    WiFi.config(ip, subnet, gateway);
  }
  
  WiFi.mode(WIFI_STA);

  // Wait until wifi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  // Print debug info via serial
  Serial.println();
  Serial.print("Connected - IP: ");
  Serial.println(WiFi.localIP());

  // Start MDNS (That you can ping your ESP8266)
  if (MDNS.begin(HOSTNAME)) {
    Serial.println("> MDNS responder started");
  }

  // Register routes
  server.on("/", index_route);
  server.on("/measure", measure_route);
  server.onNotFound(not_found_route);
  server.begin();
  
  Serial.println("> HTTP server started");

  // Turn on LED
  digitalWrite(LED, HIGH);
}

void loop(void) {
  // Update
  server.handleClient();
  MDNS.update();
}
