//LUX OSC RELAY

#include <SPI.h>
#include <EthernetENC.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 IPAddress ip(10,81,50,26);

int serverPort  = 8000;
int destPort = 9000;
int Relay_1 =2;
int Relay_2 =3;
int Relay_3 =4;
int Relay_4 =5;
int Buzzer =6;
int Relay1_State= LOW;
int Relay2_State= LOW;
int Relay3_State= LOW;
int Relay4_State= HIGH;


//Create UDP message object
EthernetUDP Udp;

void setup(){
  Ethernet.init(10);
  pinMode(Relay_1,OUTPUT);
  pinMode(Relay_2,OUTPUT);
  pinMode(Relay_3,OUTPUT);
  pinMode(Relay_4,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  Serial.begin(9600);
  Serial.println("OSC test");
  // start the Ethernet connection:
  Ethernet.begin(mac,ip);
  // print your local IP address:
  Serial.print("Arduino IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  digitalWrite(Relay_4,HIGH);
  Udp.begin(serverPort);
}

void loop(){
  //process received messages
  OSCMsgReceive();
}

void OSCMsgReceive(){                                                                     ////////////Recieve, Fill, Route////////////
  OSCMessage msgIN;
  int size;
  if((size = Udp.parsePacket())>0){
    while(size--)
      msgIN.fill(Udp.read());
    if(!msgIN.hasError()){
      msgIN.route("/OSC/port_1",toggleOnOff1);
      msgIN.route("/OSC/port_2",toggleOnOff2);
      msgIN.route("/OSC/port_3",toggleOnOff3);
     // msgIN.route("/OSC/port_4",toggleOnOff4);

    }
  }
}

void toggleOnOff1(OSCMessage &msg, int addrOffset){                                        ////////////toggle1////////////
  Relay1_State =  msg.getFloat(0);
  OSCMessage msgOUT("/OSC/port_1");
  digitalWrite(Relay_1, Relay1_State);
  Serial.println(Relay1_State);
  msgOUT.add(Relay1_State);
  if (Relay1_State) {
    Serial.println("Port1 on");
  }
  else {
    Serial.println("Port1 off");
  }
  Relay1_State = !Relay1_State;     // toggle the state from HIGH to LOW to HIGH to LOW ...
  Udp.beginPacket(Udp.remoteIP(), destPort);
  msgOUT.send(Udp); // send the feedback bytes
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOUT.empty(); // free space occupied by message
}

void toggleOnOff2(OSCMessage &msg, int addrOffset){                                         ////////////toggle2////////////
  Relay2_State =  msg.getFloat(0);
  OSCMessage msgOUT("/OSC/port_2");
  digitalWrite(Relay_2, Relay2_State);
  Serial.println(Relay2_State);
  msgOUT.add(Relay2_State);
  if (Relay2_State) {
    Serial.println("Port2 on");
  }
  else {
    Serial.println("Port2 off");
  }
  Relay2_State = !Relay2_State;
  Udp.beginPacket(Udp.remoteIP(), destPort);
  msgOUT.send(Udp);
  Udp.endPacket();
  msgOUT.empty();
}

void toggleOnOff3(OSCMessage &msg, int addrOffset){                                        ////////////toggle3////////////
  Relay3_State =  msg.getFloat(0);
  OSCMessage msgOUT("/OnOff/port_3");
  digitalWrite(Relay_3, Relay3_State);
  Serial.println(Relay3_State);
  msgOUT.add(Relay3_State);
  if (Relay3_State) {
    Serial.println("Port3 on");
    digitalWrite(Buzzer,LOW);
  }
  else {
    Serial.println("Port3 off");
    digitalWrite(Buzzer,HIGH);
  }
  Relay3_State = !Relay3_State;     // toggle the state from HIGH to LOW to HIGH to LOW ...
  Udp.beginPacket(Udp.remoteIP(), destPort);
  msgOUT.send(Udp); // send the feedback bytes
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOUT.empty(); // free space occupied by message
}

void toggleOnOff4(OSCMessage &msg, int addrOffset){                                        ////////////toggle4////////////
  Relay4_State =  msg.getFloat(0);
  OSCMessage msgOUT("/OnOff/port_4");
  digitalWrite(Relay_4, Relay4_State);
  Serial.println(Relay4_State);
  msgOUT.add(Relay4_State);
  if (Relay4_State) {
    Serial.println("Port4 on");
  }
  else {
    Serial.println("Port4 off");
  }
  Relay4_State = !Relay4_State;     // toggle the state from HIGH to LOW to HIGH to LOW ...
  Udp.beginPacket(Udp.remoteIP(), destPort);
  msgOUT.send(Udp); // send the feedback bytes
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOUT.empty(); // free space occupied by message
}

void blinkPort(){
digitalWrite(Relay_3, HIGH);
delay(1000);
digitalWrite(Relay_3,LOW);
delay(1000);
}
