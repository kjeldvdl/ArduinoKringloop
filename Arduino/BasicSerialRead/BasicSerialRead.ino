int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(115200);     // opens serial port, sets data rate to 9600 bps
        Serial.println("Basis test Serial read");
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
        } else {
          Serial.println("Geen data beschikbaar");
        }
        delay(500);
}
