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
  lcd.clear();                                         // Once handshake has been completed on game launch this block prints connected to SimPit in game and the LCD
  lcd.print("   Connected!");
  delay(5000);
  lcd.clear();
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);

  mySimpit.registerChannel(APSIDES_MESSAGE); // register input channel
  mySimpit.inboundHandler(messageHandler);  // register message handler
}

  void loop() {
mySimpit.update(); //processes incoming messages
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
    case APSIDES_MESSAGE:
      if (msgSize == sizeof(apsidesMessage)) {
        apsidesMessage myApsides;
        myApsides = parseApsides(msg);
        dtostrf(myApsides.apoapsis, 8, 0, apoapsis_str);
        dtostrf(myApsides.periapsis, 8, 0, periapsis_str);
        lcd.setCursor(0,0);
        lcd.print("AP: ");
        lcd.print(apoapsis_str);
        lcd.setCursor(0,1);
        lcd.print("PA: ");
        lcd.print(periapsis_str);
      }
      break;
  }
}

