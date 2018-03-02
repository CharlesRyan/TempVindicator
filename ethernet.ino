#include <UIPEthernet.h>
#include <UIPServer.h>
#include <UIPClient.h>
#include "utility/logging.h"

#define MACADDRESS 0x70,0x69,0x69,0x2D,0x30,0x31
#define MYIPADDR 10,0,6,160
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 10,0,0,254
#define LISTENPORT 1000
#define UARTBAUD 115200

uint8_t mac[6] = {MACADDRESS};
uint8_t myIP[4] = {MYIPADDR};
uint8_t myMASK[4] = {MYIPMASK};
uint8_t myDNS[4] = {MYDNS};
uint8_t myGW[4] = {MYGW};
int ipSlot = 10;                                                            // ipSlot through ipSlot + 3 are locations of ip address
int receiveCnt;
int transmitCnt;
char resp[8];                                                               // response to be sent via UDP
int scaledTemp1, scaledTemp2;                                               // temperature value after application of scaling table
byte temp11, temp12, temp21, temp22;                                        // two scaled temperature values translated to hex and split into two values for transmission

EthernetServer server = EthernetServer(LISTENPORT);

void ledBlinkRG(int idx) {
  for (int i = 0; i < 3; i++) {
    leds[idx] = CRGB(255, 0, 0);
    FastLED.show();
    delay(100);
    leds[idx] = CRGB(0, 255, 0);
    FastLED.show();
    delay(100);
  }
}

void lanStart() {
    #if ACTLOGLEVEL>LOG_NONE
    #if defined(ARDUINO)
      LogObject.begin(UARTBAUD);
    #endif
    #if defined(__MBED__)
      Serial LogObject(SERIAL_TX,SERIAL_RX);
      LogObject.baud(UARTBAUD);
    #endif
  #endif
  
  // initialize the ethernet device
  Ethernet.begin(mac,myIP,myDNS,myGW,myMASK);
  server.begin();
  rgbCycle();
}

void clientWatch(){
  size_t size;

  if (EthernetClient client = server.available())
    {
      if (client)
        {
          while((size = client.available()) > 0)
            {
              uint8_t* msg = (uint8_t*)malloc(size);
              size = client.read(msg,size);
              #if ACTLOGLEVEL>=LOG_INFO
                LogObject.uart_send_buf_len(msg,size);
              #endif
              client.write(msg,size);
              free(msg);
            }
        }
    }
}

// retrieve IP from EEPROM
void retrieveIP() {
  if(EEPROM.read(ipSlot) != 0){
    myip[0] = EEPROM.read(ipSlot);
    myip[1] = EEPROM.read(ipSlot + 1);
    myip[2] = EEPROM.read(ipSlot + 2);
    myip[3] = EEPROM.read(ipSlot + 3);
  }
}

// store IP in EEPROM
void storeIP() {
  EEPROM.write(ipSlot, myip[0]);
  EEPROM.write(ipSlot + 1, myip[1]);
  EEPROM.write(ipSlot + 2, myip[2]);
  EEPROM.write(ipSlot + 3, myip[3]);
}

void genBCH(byte curByte, byte* bch)
{
  int i;
  *bch ^= curByte; // XOR w/byte xmtd

  for ( i = 0; i < 8; i++ ) {
    if ((*bch & 1) == 1)
      *bch = (*bch >> 1) ^ 0xB8; // shl and XOR w/BCH poly
    else
      *bch = (*bch >> 1);
  }
}

byte genRespondBCH(byte *buff, int count)
{
  byte bch = 0, fBCHpoly;
  int i;

  for ( i = 0; i < count; i++ ) {
    genBCH(buff[i], &bch);
  }

  bch ^= 0xFF; // XOR w/final BCH poly

  // store it and return it
  buff[i] = bch;
  return (bch);
}

// increment IP address at position idx
void incIP(int idx) {
  if (myip[idx] < 255) {
    myip[idx]++;
    storeIP();
  }
}

// decrement IP address at position idx
void decIP(int idx) {
  if (myip[idx] > 0) {
    myip[idx]--;
    storeIP();
  }
}

int getIP() {
  return myIP;
}

int getTx() {
  return transmitCnt;
}

int getRx() {
  return receiveCnt;
}
