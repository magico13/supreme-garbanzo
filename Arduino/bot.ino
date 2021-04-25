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

void motors_disable()
{
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, LOW);

    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, LOW);
}

void motors_forward()
{
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, HIGH);

    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, HIGH);
}

void motors_backward()
{
    digitalWrite(MOTOR_A_1, HIGH);
    digitalWrite(MOTOR_A_2, LOW);

    digitalWrite(MOTOR_B_1, HIGH);
    digitalWrite(MOTOR_B_2, LOW);
}

void motors_left() //TODO: Confirm
{
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, HIGH);

    digitalWrite(MOTOR_B_1, HIGH);
    digitalWrite(MOTOR_B_2, LOW);
}

void motors_right() //TODO: Confirm
{
    digitalWrite(MOTOR_A_1, HIGH);
    digitalWrite(MOTOR_A_2, LOW);

    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, HIGH);
}

// speed is between 0 and 100 percent
void motor_speed(int speed)
{
    speed = constrain(speed, 0, 100);
    speed *= 2.55f; //scale 0 to 255, drop remainder to int

    analogWrite(PWM_MOTOR_A, speed);
    analogWrite(PWM_MOTOR_B, speed);
}

// the setup function runs once when you press reset or power the board
void setup() 
{
    Serial.begin(115200);

    // set up motor pins
    pinMode(PWM_MOTOR_A, OUTPUT);
    pinMode(PWM_MOTOR_B, OUTPUT);

    pinMode(MOTOR_A_1, OUTPUT);
    pinMode(MOTOR_A_2, OUTPUT);

    pinMode(MOTOR_B_1, OUTPUT);
    pinMode(MOTOR_B_2, OUTPUT);

    // set initial motor conditions
    motors_forward();
    motor_speed(0);
}

// the loop function runs over and over again forever
void loop() 
{
    //Serial.println( hc.dist() );
    float dist = hc.dist();
    if (dist < 15)
    { // stop, spin around to the left for 0.1s, start going again
        motor_speed(0);
        motors_left();
        motor_speed(50);
        delay(100);
        motor_speed(0);
        motors_forward();
    }
    else if (dist < 50)
    { // slow down to 25%
        motor_speed(25);
    }
    else
    { // go at 50% speed, decently quick
        motor_speed(50);
    }
}
