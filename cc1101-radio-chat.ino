//
// CC1101 radio chat device
// allows for bidirectioal half duplex communication
// on selected radio channel, modulation, ..
//
// (C) Adam Loboda '2021
//
// based on great SmartRC library by Little_S@tan
// Please download ZIP from 
// https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
// and attach it as ZIP library for Arduino
//
// This code will ONLY work with Arduino Pro Micro 3.3V 8MHz
//



#include <ELECHOUSE_CC1101_SRC_DRV.h>


#define CCBUFFERSIZE 64

// buffer for receiving  CC1101
byte ccreceivingbuffer[CCBUFFERSIZE] = {0};

// buffer for sending  CC1101
byte ccsendingbuffer[CCBUFFERSIZE] = {0};

// The RX LED has a defined Arduino Pro Micro pin
int RXLED = 17; 


void setup() {

     // initialize USB Serial Port CDC
     Serial.begin(9600);

     while (!Serial) {
        ; // wait until USB CDC port connects... Needed for Leonardo only
                     }
     Serial.println("CC1101 Radio chat started...");
     Serial.println();  // print CRLF

     // Arduino Pro Micro - RXLED diode will be used for debug blinking
     pinMode(RXLED, OUTPUT);  // Set RX LED as an output

     // Following section enables SmartRC CC1101 library 
     // to work with Arduino Pro Micro
     // if using different board, please remove it
     // defining PINs set for Arduino Pro Micro setup
     byte sck = 15;   
     byte miso = 14;
     byte mosi = 16;
     byte ss = 10;
     int gdo0 = 3;
     // initializing library with custom pins selected
     ELECHOUSE_cc1101.setSpiPin(sck, miso, mosi, ss);

    // Main part to tune CC1101 with proper frequency, modulation and encoding    
    ELECHOUSE_cc1101.Init();                // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setGDO0(gdo0);         // set lib internal gdo pin (gdo0). Gdo2 not use for this example.
    ELECHOUSE_cc1101.setCCMode(1);          // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(1);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);        // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setDeviation(47.60);   // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
    ELECHOUSE_cc1101.setChannel(0);         // Set the Channelnumber from 0 to 255. Default is cahnnel 0.
    ELECHOUSE_cc1101.setChsp(199.95);       // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
    ELECHOUSE_cc1101.setRxBW(812.50);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
    ELECHOUSE_cc1101.setDRate(1.2);         // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    ELECHOUSE_cc1101.setPA(10);             // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setSyncMode(2);        // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setSyncWord(211, 145); // Set sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low)
    ELECHOUSE_cc1101.setAdrChk(0);          // Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.
    ELECHOUSE_cc1101.setAddr(0);            // Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).
    ELECHOUSE_cc1101.setWhiteData(0);       // Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.
    ELECHOUSE_cc1101.setPktFormat(0);       // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
    ELECHOUSE_cc1101.setLengthConfig(1);    // 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved
    ELECHOUSE_cc1101.setPacketLength(0);    // Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.
    ELECHOUSE_cc1101.setCrc(1);             // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    ELECHOUSE_cc1101.setCRC_AF(0);          // Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.
    ELECHOUSE_cc1101.setDcFilterOff(0);     // Disable digital DC blocking filter before demodulator. Only for data rates ≤ 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).
    ELECHOUSE_cc1101.setManchester(1);      // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
    ELECHOUSE_cc1101.setFEC(0);             // Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.
    ELECHOUSE_cc1101.setPRE(3);             // Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24
    ELECHOUSE_cc1101.setPQT(0);             // Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4∙PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
    ELECHOUSE_cc1101.setAppendStatus(0);    // When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.

}


void loop() {

  // index for serial port characters
  int i = 0;
   
   //Checks whether something has been received.
  if (ELECHOUSE_cc1101.CheckReceiveFlag())
      {
       // blink LED RX - only for Arduino Pro Micro
       digitalWrite(RXLED, LOW);   // set the RX LED ON

       //CRC Check. If "setCrc(false)" crc returns always OK!
       if (ELECHOUSE_cc1101.CheckCRC())
          { 
            //Get received Data and calculate length
            int len = ELECHOUSE_cc1101.ReceiveData(ccreceivingbuffer);
            // put NULL at the end of char buffer
            ccreceivingbuffer[len] = '\0';

            //Print received in char format.
            Serial.print((char *) ccreceivingbuffer);
          };   // end of CRC check IF

       // blink LED RX - only for Arduino Pro Micro
       digitalWrite(RXLED, HIGH);   // set the RX LED OFF

      };   // end of Check receive flag if
 

       // chcecking if anything received over Serial port has to be send over radio
       // if some chars available, copy it into CC sending buffer

       // check if there are available chars in serial port buffer
       if (Serial.available())
          { 
            // blink LED RX - only for Arduino Pro Micro
            digitalWrite(RXLED, LOW);   // set the RX LED ON
            
            // clear serial port buffer index
            i = 0;

            // something was received over serial port put it into radio sending buffer
            while (Serial.available() and (i<(CCBUFFERSIZE-1)) ) 
             {
              // read single character from Serial port         
              ccsendingbuffer[i] = Serial.read();

              // also put it as ECHO back to serial port
              Serial.write(ccsendingbuffer[i]);
               
              // if CR was received add also LF character and display it on Serial port
              if (ccsendingbuffer[i] == 0x0d )
                  {  
                    Serial.write( 0x0a );
                    i++;
                    ccsendingbuffer[i] = 0x0a;
                  }
              //
              
              // increase CC1101 TX buffer position
              i++;   
             };

            // put NULL at the end of CC transmission buffer
            ccsendingbuffer[i] = '\0';

            // send these data to radio over CC1101
            ELECHOUSE_cc1101.SendData(ccsendingbuffer);

            // blink LED RX - only for Arduino Pro Micro
            digitalWrite(RXLED, HIGH);   // set the RX LED OFF

         };
 
}  // end of main LOOP
