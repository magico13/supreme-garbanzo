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

bool motors_enabled = false;
bool distance_measure = false;

void motors_disable()
{
    motors_enabled = false;
    motor_speed(0);
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, LOW);

    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, LOW);
}

void motors_enable()
{
    motors_enabled = true;
}

// speed is between 0 and 100 percent
void motor_speed(int speed)
{
    motor_speed_left(speed);
    motor_speed_right(speed);
}

int scale_speed(int speed)
{
    speed = constrain(speed, -100, 100);
    speed *= 2.55f; //scale 0 to 255, drop remainder to int
    return speed;
}

void motor_speed_left(int speed)
{
    speed = scale_speed(speed);
    if (speed < 0)
    {
        digitalWrite(MOTOR_A_1, HIGH);
        digitalWrite(MOTOR_A_2, LOW);
    }
    else
    {
        digitalWrite(MOTOR_A_1, LOW);
        digitalWrite(MOTOR_A_2, HIGH);
    }
    if (motors_enabled) analogWrite(PWM_MOTOR_A, abs(speed));
}

void motor_speed_right(int speed)
{
    speed = scale_speed(speed);
    if (speed < 0)
    {
        digitalWrite(MOTOR_B_1, HIGH);
        digitalWrite(MOTOR_B_2, LOW);
    }
    else
    {
        digitalWrite(MOTOR_B_1, LOW);
        digitalWrite(MOTOR_B_2, HIGH);
    }
    if (motors_enabled) analogWrite(PWM_MOTOR_B, abs(speed));
}

void drive(int leftSpeed, int rightSpeed, unsigned long time)
{
    motor_speed_left(leftSpeed);
    motor_speed_right(rightSpeed);
    delay(time);
    motor_speed(0);
}

void turn_in_place(bool right, unsigned long time, int speed)
{
    if (right) drive(speed, -speed, time);
    else drive(-speed, speed, time);
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
    motors_disable();
}

// the loop function runs over and over again forever
void loop() 
{
    float dist = 0;
    if (distance_measure)
    {
        dist = hc.dist();
        if (dist > 2000)
        {
            dist = -1;
        }
    }

    if (Serial.available() > 0) 
    {
        String input = Serial.readStringUntil('\n');
        if (input.startsWith("m_"))
        { //motor commands
            if (input == "m_0")
            {
                motors_disable();
                // Serial.println("Motors disabled");
            }
            else if (input == "m_1")
            {
                motors_enable();
                // Serial.println("Motors enabled");
            }
        }
        else if (input.startsWith("s_"))
        { //speed commands
            //s_50_50_1000 would be left and right motor at 50% for 1000 ms
            //s_25_50_250 would be left at 25% and right at 50% (turning left) for 250ms
            int firstUnderscore = input.indexOf('_', 2);
            long left = input.substring(2, firstUnderscore).toInt();
            int secondUnderscore = input.indexOf('_', firstUnderscore+1);
            long right = input.substring(firstUnderscore+1, secondUnderscore).toInt();
            long time = input.substring(secondUnderscore+1).toInt();

            // Serial.print(left);
            // Serial.print(" ");
            // Serial.print(right);
            // Serial.print(" ");
            // Serial.println(time);

            drive(left, right, time);
        }
        else if (input.startsWith("d_"))
        {
            if (input == "d_0")
            {
                distance_measure = false;
                // Serial.println("Distance sensor disabled");
            }
            else if (input == "d_1")
            {
                distance_measure = true;
                // Serial.println("Distance sensor enabled");
            }
        }
    }

    // print a bunch of data
    Serial.print((int)motors_enabled);
    Serial.print(" ");
    Serial.print(dist);
    Serial.println();
}
