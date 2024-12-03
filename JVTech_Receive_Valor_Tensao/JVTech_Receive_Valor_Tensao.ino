
// include the library
#include <RadioLib.h>

// SX1276 JVTech has the following connections:
// NSS pin:   5
// DIO0 pin:  26
// RESET pin: 14
// DIO1 pin:  3
SX1276 radio = new Module(5, 26, 14, 3);


void setup() {
  Serial.begin(9600);

  // initialize SX1276 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  radio.setFrequency(915.2);
  radio.setBandwidth(125.0);
  radio.setSpreadingFactor(9);
  radio.setCodingRate(8);
  radio.setOutputPower(22);

  // when new packet is received
  radio.setPacketReceivedAction(setFlag);

  // start listening for LoRa packets
  //Serial.print(F("[SX1276] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

}

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we got a packet, set the flag
  receivedFlag = true;
}

void loop() {
  // check if the flag is set
  if(receivedFlag) {
    // reset flag
    receivedFlag = false;

    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);

    // you can also read received data as byte array
    /*
      byte byteArr[8];
      int numBytes = radio.getPacketLength();
      int state = radio.readData(byteArr, numBytes);
    */

    if (state == RADIOLIB_ERR_NONE) {
      int buttonState = 3;
      // print out the state of the button:
      Serial.println(buttonState);
      delay(3);  // delay in between reads for stability
      // packet was successfully received
      
      //Serial.println(F("[SX1278] Received packet!"));

      // print data of the packet
      //Serial.print(F("[SX1278] Data:\t\t"));
      //Serial.println(str);

      // print RSSI (Received Signal Strength Indicator)
      //Serial.print(F("[SX1278] RSSI:\t\t"));
      //Serial.print(radio.getRSSI());
      //Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      //Serial.print(F("[SX1278] SNR:\t\t"));
      //Serial.print(radio.getSNR());
     // Serial.println(F(" dB"));

      // print frequency error
      //Serial.print(F("[SX1278] Frequency error:\t"));
      //Serial.print(radio.getFrequencyError());
      //Serial.println(F(" Hz"));

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("[SX1278] CRC error!"));

    } else {
      // some other error occurred
      Serial.print(F("[SX1278] Failed, code "));
      Serial.println(state);

    }
  }
}



