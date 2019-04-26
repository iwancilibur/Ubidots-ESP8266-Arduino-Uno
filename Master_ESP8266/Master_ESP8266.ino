/****************************************
 * Include Libraries
 ****************************************/
#include <UbidotsESP8266.h>
int indikator=LED_BUILTIN;
/****************************************
 * Define Constants
 ****************************************/
namespace {
  const char * WIFISSID = "www.interactiverobotics.club3"; // Assign your WiFi SSID
  const char * PASSWORD = "cilibur2019"; // Assign your WiFi password
  const char * TOKEN = "BBFF-bFkIp9nrK1gDOsRqJe92tRQPCysdZx"; // Assign your Ubidots TOKEN
}

Ubidots client(TOKEN);

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  pinMode(indikator,OUTPUT);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  client.readData(); // Reads the command from the logger
  digitalWrite(indikator,HIGH);
  delay(800);
  digitalWrite(indikator,LOW);
  delay(200);
}
