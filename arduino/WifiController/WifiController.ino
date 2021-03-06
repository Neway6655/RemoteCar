#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <AFMotor.h>
#include <String.h>
#include "utility/debug.h"
#include "utility/socket.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed

#define WLAN_SSID       "AndroidAP"           // cannot be longer than 32 characters!
#define WLAN_PASS       "12345678"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           18001    // What TCP port to listen on for connections.  The echo protocol uses port 7.

Adafruit_CC3000_Server carServer(LISTEN_PORT);

// DC motor on M1
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);

int fastSpeed = 120;
int slowSpeed = 80;

void setup(void)
{
  Serial.begin(9600);
  
  motor1.setSpeed(fastSpeed);
  motor2.setSpeed(fastSpeed);
  motor1.run(RELEASE);
  motor2.run(RELEASE); 
  
  Serial.println(F("Hello, CC3000!\n")); 

  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  
  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }
  
  // Start listening for connections
  carServer.begin();
  
  Serial.println(F("Listening for connections..."));
}

void accelerate() {
  motor1.setSpeed(fastSpeed);
  motor2.setSpeed(fastSpeed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void decelerate() {
  motor1.setSpeed(slowSpeed);
  motor2.setSpeed(slowSpeed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void turnLeft() {
  motor1.setSpeed(slowSpeed);
  motor1.run(FORWARD);
  motor2.run(RELEASE);
}

void turnRight(){
  motor2.setSpeed(slowSpeed);
  motor1.run(RELEASE);
  motor2.run(FORWARD);
}

void doStop(){
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

char buffer[16];

void loop(void)
{
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = carServer.available();
  if (client) {
     // Check if there is data available to read.
     if (client.available() > 0) {
       Serial.println(F("Client connec..."));
       // Read.
       int n = client.read(buffer, 15, 1);
       if (n < 0){
         Serial.println(F("Error data received..."));
       }
       client.write(buffer,15,1);
       Serial.println(buffer);
       if (strcmp(buffer,"f") == 0) {
         Serial.println(F("forward..."));
         accelerate();
       }else if (strcmp(buffer,"b") == 0){
         Serial.println(F("backward..."));
         decelerate();
       }else if (strcmp(buffer,"l") == 0){
         Serial.println(F("turn left..."));
         turnLeft();
       }else if (strcmp(buffer,"r") == 0){
         Serial.println(F("turn right..."));
         turnRight();
       }else if (strcmp(buffer,"s") == 0){
         doStop();
       }
       memset(buffer,'\0',sizeof(buffer));
     }
  }
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
