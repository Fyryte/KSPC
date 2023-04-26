void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch (messageType) {
    case APSIDES_MESSAGE:
      if (msgSize == sizeof(apsidesMessage)) {
        apsidesMessage myApsides;
        myApsides = parseApsides(msg);
        dtostrf(myApsides.apoapsis, 8, 0, apoapsis_str);
        dtostrf(myApsides.periapsis, 8, 0, periapsis_str);
        lcd.clear(); // clear the LCD screen
        lcd.setCursor(0, 0);

        if (atol(apoapsis_str) >= 1000 || atol(apoapsis_str) <= -1000) {
          float apoapsis = atof(apoapsis_str) / 1000.0;
          char apoapsis_buf[8];
          dtostrf(round(apoapsis * 100) / 100.0, 6, 2, apoapsis_buf);
          lcd.print("AP: ");
          lcd.print(apoapsis_buf);
          lcd.print(" KM");
        } else {
          lcd.print("AP: ");
          lcd.print(apoapsis_str);
          lcd.print(" M");
        }

        lcd.setCursor(0, 1);

        if (atol(periapsis_str) >= 1000 || atol(periapsis_str) <= -1000) {
          float periapsis = atof(periapsis_str) / 1000.0;
          char periapsis_buf[8];
          dtostrf(round(periapsis * 100) / 100.0, 6, 2, periapsis_buf);
          lcd.print("PA: ");
          lcd.print(periapsis_buf);
          lcd.print(" KM");
        } else {
          lcd.print("PA: ");
          lcd.print(periapsis_str);
          lcd.print(" M");
        }
      }
      break;
  }
}
