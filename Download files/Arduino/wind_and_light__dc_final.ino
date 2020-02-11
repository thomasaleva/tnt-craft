#include <SerialCraft.h>

SerialCraft sc;
const int ledPinHouse1 = 13;    //Lantern houses
const int ledPinSolar = 8;      //Solar production
const int ledPinGreen = 4;      //Storage green
const int ledPinOrange = 7;     //Storage orange
const int ledPinRed = 12;       //Storage red
const int redstoneDC1Pin = 10;  //Motor pin WT
const int redstoneDC2Pin = 11;  //Motor pin HT

void setup() {
sc.setup();

// Arduino LED control
sc.registerDigitalInputCallback(sendMessage, 2);    // Serialcraft command to send message based on signal from digital port 2
sc.registerAnalogInputCallback(analogCallback, A0); // Serialcraft command to send message based on signal from analog port A0
sc.registerRedstoneSerialCallback(redstoneSerial);  // Serialcraft command to start RedstoneSerialMessage block connection

pinMode(ledPinHouse1, OUTPUT);    // Set pin to output
pinMode(ledPinSolar, OUTPUT);     // Set pin to output
pinMode(ledPinRed, OUTPUT);       // Set pin to output
pinMode(ledPinOrange, OUTPUT);    // Set pin to output
pinMode(ledPinGreen, OUTPUT);     // Set pin to output
// Serial DC Control 
  pinMode(redstoneDC1Pin, OUTPUT);
  pinMode(redstoneDC2Pin, OUTPUT);    
}

void loop() {
    sc.loop();
}

// Serial DC Control to map the input from the turbines in Minecraft,
// that will make the dc motors rotate in variable speeds

// Each READING will refer to their own command from Void Setup

void redstoneSerial(int s, String id) { 
    if(id == "WT") {                        // Will retrieve value from RedstoneSerialMessage block with ID WT
        analogWrite(redstoneDC1Pin, map(s, 0, 15, 0, 220));   // Maps 0-15 value to 0-255
    } if(id == "HT") {                      // Will retrieve value from RedstoneSerialMessage block with ID HT
        analogWrite(redstoneDC2Pin, map(s, 0, 15, 0, 200));   // Maps 0-15 value to 0-255
        }   
}
// The on/off switch will send a signal to Minecraft that will create a chat message.
// The chatmessage will send the command for  clear weather or rainy weather depending on the state of the switch.

void sendMessage(bool reading) {
  if(reading == HIGH) {
    sc.sendChatMessage("/weather clear");
  }
  if(reading == LOW) {
    sc.sendChatMessage("/weather rain");
  }
 }
 
// Light sensor
// The light sensor will read a value between 50 and 625 depending on real world brightness (Adjust to your own setting with the assist of Serial Monitor)
// Using this you can make it day and night in Minecraft through the chat message
// LEDs in the real world are also connected and will go on and off simultaneously with the lights in minecraft

void analogCallback(int reading) {
  int t = map(reading, 50, 625, 18000, 6500);
  sc.setTime(t);

// This will control the lights on the Arduino (works around Minecraft, can be done with RedstoneSerialMessage)
  if (reading < 450) {
    digitalWrite(ledPinHouse1, HIGH);
  }
  if (reading > 450) {
    digitalWrite(ledPinHouse1, LOW);
    digitalWrite(ledPinGreen, HIGH);
    digitalWrite(ledPinOrange, LOW);
    digitalWrite(ledPinRed, LOW);
  }
  if ((reading > 300) && (reading < 450)) {
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinOrange, HIGH);
    digitalWrite(ledPinRed, LOW);
  }
     if (reading < 300) {
    digitalWrite(ledPinSolar, LOW);
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinOrange, LOW);
    digitalWrite(ledPinRed, HIGH);
  }

//This will turn on the Solar cell LED (same workaround as stated above) 
if (reading > 343){
 

 digitalWrite(ledPinSolar, HIGH);
}
  }
