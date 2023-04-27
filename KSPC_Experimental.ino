#include <LiquidCrystal_I2C.h>        // LCD Display
#include <KerbalSimpit.h>             // SimPit     
#include <KerbalSimpitMessageTypes.h> // SimPit
#include <PayloadStructs.h>           // SimPit

KerbalSimpit mySimpit(Serial);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

char apoapsis_str[150];  // telemetry strings
char periapsis_str[150];

void setup() {
  Serial.begin(115200);  // Initializes the serial communication
  lcd.init();             // LCD Display initialization
  lcd.backlight();
  lcd.print("   Working");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);

  while (!mySimpit.init()) {
    delay(100);  // Handshake protocol attempts every 100ms until connection
  }
  
  lcd.clear();  // Once handshake has been completed on game launch this block prints connected to SimPit in game and the LCD
  lcd.print("   Connected!");
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  mySimpit.registerChannel(APSIDES_MESSAGE);  // register input channel
  mySimpit.inboundHandler(messageHandler);    // register message handler
}

void loop() {
  mySimpit.update();  // processes incoming messages
}

// LCD MESSAGE HANDLER
void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch (messageType) {
    case APSIDES_MESSAGE:
      if (msgSize == sizeof(apsidesMessage)) {
        // AP and PA derived from APSIDES
        apsidesMessage myApsides;
        myApsides = parseApsides(msg);
        
        // Convert apoapsis and periapsis values to floats
        float apoapsis = myApsides.apoapsis / 1000.0;
        float periapsis = myApsides.periapsis / 1000.0;
        
        // Format apoapsis and periapsis values as strings
        dtostrf(apoapsis, 6, 2, apoapsis_str);
        dtostrf(periapsis, 6, 2, periapsis_str);
        
        // Display AP and PA on LCD
        lcd.clear();  // clear the LCD screen
        lcd.setCursor(0, 0);
        lcd.print("AP: ");
        lcd.print(apoapsis_str);
        lcd.print(" KM");
        lcd.setCursor(0, 1);
        lcd.print("PA: ");
        lcd.print(periapsis_str);
        lcd.print(" KM");
      }
      break;
  }
}
