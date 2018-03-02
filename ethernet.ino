#include <enc28j60.h>
#include <EtherCard.h>
#include <net.h>
#include <IPAddress.h>
#include <EEPROM.h>

int ipSlot = 10;                                       // ipSlot through ipSlot + 3 are addresses of ip address
int receiveCnt;
int transmitCnt;

// ethernet interface ip, gateway, and mac addresses
byte myip[] = { 10, 0, 6, 160 };
//byte myip[];
static byte gwip[] = { 10, 0, 0, 254 };
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[100]; // tcp/ip send and receive buffer

void udpRespond(uint8_t * ip, uint8_t * port){
  byte response[] = "TAATFAT01T2AT10T11T01T12";
  ether.sendUdp(response, 3, 2001, ip, port);
  Serial.println("done");
}

void retrieveIP(){
    myip[0] = EEPROM.read(ipSlot);
    myip[1] = EEPROM.read(ipSlot + 1);
    myip[2] = EEPROM.read(ipSlot + 2);
    myip[3] = EEPROM.read(ipSlot + 3);
}

void storeIP() {
    EEPROM.write(ipSlot, myip[0]);
    EEPROM.write(ipSlot + 1, myip[1]);
    EEPROM.write(ipSlot + 2, myip[2]);
    EEPROM.write(ipSlot + 3, myip[3]);
}

// increment IP address at position idx
void incIP(int idx){
  myip[idx]++;
  storeIP();
//  ether.printIp(myip);
}

// decrement IP address at position idx
void decIP(int idx){
  myip[idx]--;
  storeIP();
//  ether.printIp(myip);
}

//callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
  
  Serial.print("dest_port: ");
  Serial.println(dest_port);
  Serial.print("src_port: ");
  Serial.println(src_port);
  
  Serial.println("src_ip: ");
  ether.printIp(src_ip);
  Serial.println("data: ");
  Serial.println(data);
  receiveCnt++;
  ledBlink(1);
  udpRespond(src_ip, src_port);
}

void ledBlink(int idx) {
  for (int i = 0; i < 3; i++) {
    leds[idx] = CRGB(255, 0, 0);
    FastLED.show();
    delay(100);
    leds[idx] = CRGB(0, 255, 0);
    FastLED.show();
    delay(100);
  }
}

void lanStart(){
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) {
    Serial.println( "Failed to access Ethernet controller");
  }
  retrieveIP();
  ether.staticSetup(myip, gwip); 
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  //register udpSerialPrint() to port 2001
  ether.udpServerListenOnPort(&udpSerialPrint, 2001);

  //register udpSerialPrint() to port 19.
  ether.udpServerListenOnPort(&udpSerialPrint, 19);
}

void lanPacketLoop(){
  ether.packetLoop(ether.packetReceive());
}

int getIP(){
  return myip;
}

int getTx(){
  return transmitCnt;
}

int getRx(){
  return receiveCnt;
}





