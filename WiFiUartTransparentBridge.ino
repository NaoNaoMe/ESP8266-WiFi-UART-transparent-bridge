//******************************************************************************
// WiFiUartTransparentBridge
// Version 1.0.0
// Note
// This sketch is based on "WiFiTelnetToSerial - Example Transparent UART to Telnet Server for esp8266"
//******************************************************************************

#include <ESP8266WiFi.h>
#include "config.h"

WiFiServer server(networkport);
WiFiClient serverClient;

void setup() {
  Serial.begin(baudrate);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  
  //start UART and the server
  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'Uart-WiFi Bridge ");
  Serial.print(WiFi.localIP());
  Serial.println(" to connect");

}

void loop() {
  //check if there are any new clients
  if (server.hasClient()){
    if (!serverClient.connected()){
      if(serverClient) serverClient.stop();
      serverClient = server.available();
    }
  }
      
  //check a client for data
  if (serverClient && serverClient.connected()){
    if(serverClient.available()){
      size_t len = serverClient.available();
      uint8_t sbuf[len];
      serverClient.readBytes(sbuf, len);
      Serial.write(sbuf, len);      
    }
  }

  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    if (serverClient && serverClient.connected()){
      serverClient.write(sbuf, len);
    }
  }
}
