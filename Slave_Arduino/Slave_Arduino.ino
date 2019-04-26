/****************************************
 * Define Constants
 ****************************************/
 #include <SoftwareSerial.h>

 SoftwareSerial koneksi(4,5);
 int idikator=13;
 namespace {
  bool flow_control = true; // control the flow of the requests
  const char * USER_AGENT = "UbidotsESP8266"; // Assgin the user agent
  const char * VERSION =  "1.0"; // Assign the version
  const char * METHOD = "POST"; // Set the method
  const char * TOKEN = "BBFF-bFkIp9nrK1gDOsRqJe92tRQPCysdZx"; // Assign your Ubidots TOKEN
  const char * DEVICE_LABEL = "ESP8266"; // Assign the device label
  const char * VARIABLE_LABEL = "temp"; // Assign the variable label
  const char * VARIABLE_LABEL2 = "temp2";
  const char * VARIABLE_LABEL3 = "temp3";
 }

char telemetry_unit[100]; // response of the telemetry unit

/* Space to store values to send */
char str_sensor1[10];
char str_sensor2[10];
char str_sensor3[10];

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  koneksi.begin(115200);
  pinMode (idikator,OUTPUT);
}

void loop() {
  char* command = (char *) malloc(sizeof(char) * 128);
  /* Wait for the server response to read the values and built the command */
  /* While the flag is true it will take the sensors readings, build the command,
     and post the command to Ubidots */
  if (flow_control) {
    /* Analog reading */
    float sensor1 = analogRead(A0);
    float sensor2 = analogRead(A1);
    float sensor3 = analogRead(A2);
    /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
    dtostrf(sensor1, 4, 2, str_sensor1);
    dtostrf(sensor2, 4, 2, str_sensor2);
    dtostrf(sensor3, 4, 2, str_sensor3);
    /* Building the logger command */
    sprintf(command, "init#");
    sprintf(command, "%s%s/%s|%s|%s|", command, USER_AGENT, VERSION, METHOD, TOKEN);
    sprintf(command, "%s%s=>", command, DEVICE_LABEL);
    sprintf(command, "%s%s:%s,%s:%s,%s:%s", command, VARIABLE_LABEL, str_sensor1, VARIABLE_LABEL2, str_sensor2,VARIABLE_LABEL3, str_sensor3);
    sprintf(command, "%s|end#final", command);

    /* Sends the command to the telemetry unit */
    koneksi.print(command);
    /* free memory*/
    free(command);
    /* Change the status of the flag to false. Once the data is sent, the status
       of the flag will change to true again */
    flow_control = false;
  }

  /* Reading the telemetry unit */
  int i = 0;
  while (koneksi.available() > 0) {
    telemetry_unit[i++] = (char)koneksi.read();
    /* Change the status of the flag; allows the next command to be built */
    flow_control = true;
  }

  if (flow_control) {
    /* Print the server response -> OK */
    Serial.write(telemetry_unit);
    /* free memory */
    //memset(telemetry_unit, 0, i);
  }
  digitalWrite(idikator,HIGH);
  delay(800);
  digitalWrite(idikator,LOW);
  delay(200); 
}
