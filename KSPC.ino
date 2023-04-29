#include <LiquidCrystal_I2C.h>        // LCD Display
#include <KerbalSimpit.h>             // SimPit     
#include <KerbalSimpitMessageTypes.h> // SimPit
#include <PayloadStructs.h>           // SimPit

KerbalSimpit mySimpit(Serial);
LiquidCrystal_I2C lcd(0x3F,16,2);
const int THROTTLE_PIN = A0; 
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured
// in miliseconds, will quickly become a bigger number than can be stored
// in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin
                                     // was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase
                                     // if the output flickers


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
  const int THROTTLE_PIN = A0; 
            
}

//processes incoming messages
void loop() {
    // Read the state of the switch into a local variable.
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // If the new button state is HIGH, that means the button
      // has just been pressed.
      if (buttonState == HIGH) {
        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.
        mySimpit.activateAction(STAGE_ACTION);
      }
    }
  }

  // Set the LED to match the state of the button.
  digitalWrite(ledPin, buttonState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  throttleMessage throttle_msg;
  // read the input on analog pin 0:
  int reading = analogRead(THROTTLE_PIN);
  // Convert it in KerbalSimpit Range
  throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);
  // print out the value you read:
  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
    // delay in between reads for stability
  delay(1);  
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

        // AP displayed above 1B meters (displayed as Gm)
        if (myApsides.apoapsis >= 1000000000 || myApsides.apoapsis < -1000000000) {
          float apoapsis = myApsides.apoapsis / 1000000000.0;
          char apoapsis_buf[16] = "AP: ";
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, &apoapsis_buf[4]);
          strncat(apoapsis_buf, " Gm", 3);
          lcd.print(apoapsis_buf);
        } 
        
        // AP displayed above 1M meters (displayed as mM)
        else if (myApsides.apoapsis >= 1000000 || myApsides.apoapsis < -1000000) {
          float apoapsis = myApsides.apoapsis / 1000000.0;
          char apoapsis_buf[16] = "AP: ";
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, &apoapsis_buf[4]);
          strncat(apoapsis_buf, " mM", 3);
          lcd.print(apoapsis_buf);
        } 

        // AP displayed above 1000 meters (displayed as M)
        else if (myApsides.apoapsis >= 1000 || myApsides.apoapsis < -1000) {
          float apoapsis = myApsides.apoapsis / 1000.0;
          char apoapsis_buf[16] = "AP: ";
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, &apoapsis_buf[4]);
          strncat(apoapsis_buf, " KM", 3);
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
        // PA displayed above 1B meters (displayed as Gm)
        if (myApsides.periapsis >= 1000000000 || myApsides.periapsis < -1000000000) {
          float periapsis = myApsides.periapsis / 1000000000.0;
          char periapsis_buf[16] = "AP: ";
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, &periapsis_buf[4]);
          strncat(periapsis_buf, " Gm", 3);
          lcd.print(periapsis_buf);
        } 

        // PA displayed above 1M meters (displayed as mM)
        else if (myApsides.periapsis >= 1000000 || myApsides.periapsis < -1000000) {
          float periapsis = myApsides.periapsis / 1000000.0;
          char periapsis_buf[16] = "AP: ";
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, &periapsis_buf[4]);
          strncat(periapsis_buf, " mM", 3);
          lcd.print(periapsis_buf);
        } 
        
        // PA displayed above 1000 meters (displayed as KM)
        else if (myApsides.periapsis >= 1000 || myApsides.periapsis < -1000) {
          float periapsis = myApsides.periapsis / 1000.0;
          char periapsis_buf[16] = "PA: ";
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, &periapsis_buf[4]);
          strncat(periapsis_buf, " KM", 3);
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
