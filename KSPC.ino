#include <LiquidCrystal_I2C.h>        // LCD Display
#include <KerbalSimpit.h>             // SimPit     
#include <KerbalSimpitMessageTypes.h> // SimPit
#include <PayloadStructs.h>           // SimPit


KerbalSimpit mySimpit(Serial);
LiquidCrystal_I2C lcd(0x3F,16,2);

char apoapsis_str[150];  // telemetry strings
char periapsis_str[150];

void setup() {
    while (!mySimpit.init()) {         
    delay(100);                         // Handshake protocol attempts ever 100ms until connection   
     Serial.begin(115200);              // Initializes the serial communication
  lcd.init();                           // LCD Display for the rest until the end of bracket
  lcd.backlight();
  lcd.print("   Working");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);


  }
  lcd.clear();                          // Once handshake has been completed on game launch this block prints connected to SimPit in game and the LCD
  lcd.print("   Connected!");
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);

  mySimpit.registerChannel(APSIDES_MESSAGE); // register input channel
  mySimpit.inboundHandler(messageHandler);   // register message handler
}

  void loop() {
    mySimpit.update(); //processes incoming messages
}
//---------------------------------------------------------------------------------LCD MESSAGE HANDLER---------------------------------------------------------------------------------//
void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
    case APSIDES_MESSAGE:
      if (msgSize == sizeof(apsidesMessage)) {                                  // AP and PA derived from APSIDES
        apsidesMessage myApsides;
        myApsides = parseApsides(msg);
        dtostrf(myApsides.apoapsis, 8, 0, apoapsis_str);
        dtostrf(myApsides.periapsis, 8, 0, periapsis_str);
        lcd.clear(); // clear the LCD screen
        lcd.setCursor(0, 0);

        if (atoi(apoapsis_str) >= 1000) {                                       // AP displayed below 1000 meters (displayed as M)
          float apoapsis = atof(apoapsis_str) / 1000.0;
          char apoapsis_buf[8];
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, apoapsis_buf);
          lcd.print("AP: ");
          lcd.print(apoapsis_buf);
          lcd.print(" KM");
        }
        else if (atoi(apoapsis_str) < -1000) {                                  // AP displayed below -1000-0 meters (displayed as KM)
         float apoapsis = atof(apoapsis_str) / 1000.0;
         char apoapsis_buf[8];
         dtostrf(round(apoapsis * 100) / 100.0, 6, 2, apoapsis_buf);
          lcd.print("AP: ");
         lcd.print(apoapsis_buf);
          lcd.print(" KM");
        } 
        else {                                                                  // AP displayed above 1000 meters (displayed as KM)
          lcd.print("AP: ");
          lcd.print(apoapsis_str);
          lcd.print(" M");
        }

        lcd.setCursor(0, 1);
        if (atoi(periapsis_str) >= 1000) {                                      // PA displayed below 1000 meters (displayed as M)
          float periapsis = atof(periapsis_str) / 1000.0;
          char periapsis_buf[8];
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, periapsis_buf);
          lcd.print("PA: ");
          lcd.print(periapsis_buf);
          lcd.print(" KM");
        } 
        else if (atoi(periapsis_str) < -1000) {                                 // PA displayed above -1000-0 meters (displayed as KM)
        float periapsis = atof(periapsis_str) / 1000.0;
        char periapsis_buf[8];
        dtostrf(round(periapsis * 100) / 100.0, 6, 2, periapsis_buf);
        lcd.print("PA: ");
        lcd.print(periapsis_buf);
        lcd.print(" KM");
        }
        else {                                                                 // PA displayed below -1000 meters (displayed as KM)
          lcd.print("PA: ");
          lcd.print(periapsis_str);
          lcd.print(" M");
        }
      }
      break;
  }
}
//---------------------------------------------------------------------------------LCD MESSAGE HANDLER END---------------------------------------------------------------------------------//
