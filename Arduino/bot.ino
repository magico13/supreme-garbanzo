#include <HCSR04.h>

// Pins for ultrasonic range finder
#define TRIGGER_PIN 2
#define ECHO_PIN 3

// Pins for motor control
#define PWM_MOTOR_A 5
#define PWM_MOTOR_B 6

#define MOTOR_A_1 7
#define MOTOR_A_2 8

#define MOTOR_B_1 9
#define MOTOR_B_2 10

HCSR04 hc(TRIGGER_PIN, ECHO_PIN); // initialize HCSR04 (trig pin , echo pin)

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);

    // set up motor pins
    pinMode(PWM_MOTOR_A, OUTPUT);
    pinMode(PWM_MOTOR_B, OUTPUT);

    pinMode(MOTOR_A_1, OUTPUT);
    pinMode(MOTOR_A_2, OUTPUT);

    pinMode(MOTOR_B_1, OUTPUT);
    pinMode(MOTOR_B_2, OUTPUT);

    analogWrite(PWM_MOTOR_A, 0);
    analogWrite(PWM_MOTOR_B, 0);

    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, HIGH);

    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, HIGH);
}

// the loop function runs over and over again forever
void loop() {
    //Serial.println( hc.dist() );
    float dist = hc.dist();
    if (dist < 10)
    {
        analogWrite(PWM_MOTOR_A, 0);
        analogWrite(PWM_MOTOR_B, 0);
    }
    else if (dist < 40)
    {
        analogWrite(PWM_MOTOR_A, 64);
        analogWrite(PWM_MOTOR_B, 64);
    }
    else
    {
        analogWrite(PWM_MOTOR_A, 128);
        analogWrite(PWM_MOTOR_B, 128);
    }
}
