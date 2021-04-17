#include <HCSR04.h>

HCSR04 hc(2,3);//initialisation class HCSR04 (trig pin , echo pin)

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
    Serial.println( hc.dist() );
}
