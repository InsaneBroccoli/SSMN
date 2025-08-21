#include <Servo.h>
#include "buzzer.h"

// Pins
#define BUZZER 12
#define BUTTON_START 9
#define BUTTON_END 2
#define BUTTON_RETURNED 7
#define SERVO 3
#define ENA 10
#define MOTOR1 4
#define MOTOR2 5

// Waiting periods
#define WAIT 100
#define TIME_TO_DELIVER 100

// Positioning
#define INIT_POS 0
#define DEPLOY_POS 120

// Motors
#define SPEED 1 // 0 - 1

enum States {
    INITIAL,
    FIND_START,
    IDLE,
    MOVE_TO_END,
    RETURN_TO_START,
    DELIVER_BALL
};

Servo myservo;
States state = States::INITIAL;
bool handled;

void setup()
{
    myservo.attach(SERVO);

    pinMode(ENA, OUTPUT);
    pinMode(MOTOR1, OUTPUT);
    pinMode(MOTOR2, OUTPUT);
    
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_END, INPUT_PULLUP);
    pinMode(BUTTON_RETURNED, INPUT_PULLUP);

    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, LOW);

    attachInterrupt(digitalPinToInterrupt(BUTTON_END), IRS_stop_motor, FALLING);

    Serial.begin(9600);
}



void loop()
{
    switch (state)
    {
    case States::INITIAL:
        Serial.println("\nInitialising");
        myservo.write(INIT_POS);
        delay(15);

        state = States::FIND_START;
        handled = false;
        Serial.println("FINDING START");

        move_cw(SPEED);

        break;

    case States::FIND_START:
        if (digitalRead(BUTTON_RETURNED) == LOW )
        {
            state = States::IDLE;
            Serial.println("IDLE");

            stop_motor();
        }
        break;
    
    case States::IDLE:
        if (digitalRead(BUTTON_START) == LOW)
        {
            mario_racing();
            state = States::MOVE_TO_END;
            Serial.println("MOVE TO END");

            move_ccw(SPEED);
            crazyfrog();
        } else
          {
            //stop_motor();
          }
        break;

    case States::MOVE_TO_END:
        if (handled)
        {
            state = States::DELIVER_BALL;
            Serial.println("DELIVER BALL");
            handled = false;

            delay(1000);

            // eject
            myservo.write(DEPLOY_POS);
            level_clear();
        }
        break;

    case States::DELIVER_BALL:
        delay(TIME_TO_DELIVER);

        state = States::RETURN_TO_START;
        Serial.println("RETURN TO START");

        move_cw(SPEED);

        myservo.write(INIT_POS);
        break;

    case States::RETURN_TO_START:
        if (digitalRead(BUTTON_RETURNED) == LOW)
        {
            state = States::IDLE;
            Serial.println("IDLE");
            
            // Stop motor
            stop_motor();
        }
        break;

    default:
        break;
    }
}

void IRS_stop_motor()
{
    if (state == States::MOVE_TO_END && !handled)
    {
        stop_motor();
        Serial.println("MOTOR STOP");
        handled = true;
    }    
}

void speed_control(float speed)
{
    int trgt_speed = (int)((speed * 256) - 1);
    trgt_speed = trgt_speed < 0 ? 0 : trgt_speed;

    analogWrite(ENA, trgt_speed);
    Serial.println(trgt_speed);
}

void move_ccw(float speed) 
{   
    digitalWrite(MOTOR1, HIGH);
    digitalWrite(MOTOR2, LOW);
    speed_control(speed);
}

void move_cw(float speed) 
{   
    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, HIGH);
    speed_control(speed);
}

void stop_motor()
{
    speed_control(0);
    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, LOW);
}