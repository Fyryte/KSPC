#include <LiquidCrystal_I2C.h>        // LCD Display
#include <KerbalSimpit.h>             // SimPit     
#include <KerbalSimpitMessageTypes.h> // SimPit
#include <PayloadStructs.h>           // SimPit

KerbalSimpit mySimpit(Serial);
LiquidCrystal_I2C lcd(0x3F,16,2);
bool isFlying = false;

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();

  while (!mySimpit.init()) {         
    lcd.clear();
    lcd.print("   Working");

    // Initializes the serial communication
    Serial.begin(115200);              

    // LCD Display for the rest until the end of bracket
    delay(250);
    lcd.print(".");
    delay(250);
    lcd.print(".");
    delay(250);
    lcd.print(".");
    delay(250);
  }

  // Once handshake has been completed on game launch this block prints connected to SimPit in game and the LCD
  
  lcd.clear();                                            
  lcd.print("   Connected!");
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  
  // register input channel
  mySimpit.registerChannel(APSIDES_MESSAGE);              
  
  // register message handler
  mySimpit.inboundHandler(messageHandler);                
}

//processes incoming messages
void loop() {
  mySimpit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
    case APSIDES_MESSAGE: {
        
      // AP and PA derived from APSIDES
      if (msgSize == sizeof(apsidesMessage)) {                                  
        apsidesMessage myApsides = parseApsides(msg);
        lcd.clear(); // clear the LCD screen
        lcd.setCursor(0, 0);
      
        // AP displayed above 1000 meters (displayed as M)
        if (myApsides.apoapsis >= 1000 || myApsides.apoapsis < -1000) {
          float apoapsis = myApsides.apoapsis / 1000.0;
          char apoapsis_buf[16] = "AP: ";
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, &apoapsis_buf[4]);
          strncat(apoapsis_buf, " KM", 3);
          lcd.print(apoapsis_buf);
        } 

        // AP displayed above 1M meters (displayed as mM)
        if (myApsides.apoapsis >= 1000000 || myApsides.apoapsis < -1000000) {
          float apoapsis = myApsides.apoapsis / 1000000.0;
          char apoapsis_buf[16] = "AP: ";
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, &apoapsis_buf[4]);
          strncat(apoapsis_buf, " mM", 3);
          lcd.print(apoapsis_buf);
        } 
        // AP displayed above 1B meters (displayed as Gm)
        if (myApsides.apoapsis >= 1000000000 || myApsides.apoapsis < -1000000000) {
          float apoapsis = myApsides.apoapsis / 1000000000.0;
          char apoapsis_buf[16] = "AP: ";
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, &apoapsis_buf[4]);
          strncat(apoapsis_buf, " Gm", 3);
          lcd.print(apoapsis_buf);
        } 


        // AP displayed below 1000 meters (displayed as KM)
        else {
          char meters_string[16] = "AP: ";
          dtostrf(myApsides.apoapsis, 8, 0, &meters_string[4]);
          strncat(meters_string, " M", 3);
          lcd.print(meters_string);
        }

        lcd.setCursor(0, 1);

        // PA displayed above 1000 meters (displayed as M)
        if (myApsides.periapsis >= 1000 || myApsides.periapsis < -1000) {
          float periapsis = myApsides.periapsis / 1000.0;
          char periapsis_buf[16] = "PA: ";
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, &periapsis_buf[4]);
          strncat(periapsis_buf, " KM", 3);
          lcd.print(periapsis_buf);
        } 

        // PA displayed above 1M meters (displayed as mM)
        if (myApsides.periapsis >= 1000000 || myApsides.periapsis < -1000000) {
          float periapsis = myApsides.periapsis / 1000000.0;
          char periapsis_buf[16] = "AP: ";
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, &periapsis_buf[4]);
          strncat(periapsis_buf, " mM", 3);
          lcd.print(periapsis_buf);
        } 
        // PA displayed above 1B meters (displayed as Gm)
        if (myApsides.periapsis >= 1000000000 || myApsides.periapsis < -1000000000) {
          float periapsis = myApsides.periapsis / 1000000000.0;
          char periapsis_buf[16] = "AP: ";
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, &periapsis_buf[4]);
          strncat(periapsis_buf, " Gm", 3);
          lcd.print(periapsis_buf);
        } 

        // PA displayed below 1000 meters (displayed as KM)
        else {
          char meters_string[16] = "PA: ";
          dtostrf(myApsides.periapsis, 8, 0, &meters_string[4]);
          strncat(meters_string, " M", 3);
          lcd.print(meters_string);
          
        
      }
      break;

      }
    }
  }
}
