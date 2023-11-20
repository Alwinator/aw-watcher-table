#include <WiFiNINA.h>
#include <HCSR04.h>
#include "secrets.h"

// Define variables
const int TRIGGER_PIN = 2;   // Pin D2, Ultrasonic sensor
const int ECHO_PIN = 5;      // Pin D5, Ultrasonic sensor
const int LED = LED_BUILTIN; // Indicator LED

const int MEASURE_CORRECTION = 9; // Added to measure value since te tabletop is higher that the sensor

// Init web server with port 80
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup(void)
{
  // Enable Serial for testing purposes
  Serial.begin(115200);

  // Setup LED
  pinMode(LED, OUTPUT);

  // Setup ultrasonic sensor
  HCSR04.begin(TRIGGER_PIN, ECHO_PIN);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    // don't continue
    while (true)
      ;
  }

  // setup Hostname
  WiFi.setHostname(HOSTNAME);

  // attempt to connect to WiFi network
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(WIFI_SSID); // print the network name (SSID);
  while (status != WL_CONNECTED)
  {
    // Connect to WPA/WPA2 network.
    status = WiFi.begin(WIFI_SSID, WIFI_PSK);
    Serial.print(".");
    // wait 1 seconds for connection:
    delay(5000);
  }

  // Print debug info via serial
  Serial.println();
  Serial.print("Connected - IP: ");
  Serial.println(WiFi.localIP());

  // start the server
  server.begin();
}

void loop()
{
  WiFiClient client = server.available(); // listen for incoming clients

  if (client)
  {                               // if you get a client,
    // Serial.println("new client"); // print a message out the serial port
    String currentLine = "";      // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      { // if there's bytes to read from the client,

        digitalWrite(LED, HIGH);
        char c = client.read(); // read a byte
        if (c == '\n')
        { // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // measure the height
            double *height = HCSR04.measureDistanceCm();
            char response[50];
            sprintf(response, "{ \"table_height\": %d }", (int)(*height) + MEASURE_CORRECTION);

            // send height to client
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/json");
            client.println();
            client.println(response);
            client.println();

            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    // Serial.println("client disconnected");
    digitalWrite(LED, LOW);
  }
}