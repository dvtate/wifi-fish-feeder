#ifndef FISH_NETWORK_H
#define FISH_NETWORK_H

#include <ESP8266WiFi.h>

WiFiServer server(80);

void wifiInit(){
  
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected :D");
  
  
}

void serverInit(){
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 

}

bool checkInput(unit32_t& msRemaining, bool foodRemaining){
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
    return false;
  
  bool ret = false;
  
  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
    delay(1);
  
  // Read the first line of the request
  String request = client.readStringUntil(‘\r’);
  Serial.println(request);
  client.flush();
  /*
  // Match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }*/
  
  if (request.indexOf("FEED=TRUE") != -1)
    ret = true;
  
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.print("Minutes until fish gets fed automatically: ");
  client.println(msRemaining / 1000 / 60);
 
  client.println("<br><br>");
  client.println("Click <a href=\"/FEED=TRUE\">here</a> to feed your fish.<br>");
  
  if (!foodRemaining)
    client.println("<script>alert(\"WARNING\\n You are out of fish food!!!!!\");</script>");
  
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  return ret;

}



#endif
