#include <Servo.h>
#include "buzzer.h"

// Pins
#define LED2 13
#define BUZZER 12
#define BUTTON_START 9
#define BUTTON_END 2
#define BUTTON_RETURNED 7
#define MOTOR1 4
#define MOTOR2 5

// Waiting periods
#define WAIT 100
#define TIME_TO_DELIVER 3000

// Positioning
#define INIT_POS 0
#define DEPLOY_POS 120

enum States {
    INITIAL,
    FIND_START,
    IDLE,
    MOVE_TO_END,
    RETURN_TO_START,
    DELIVER_BALL,
    TESTING
};

Servo myservo;
States state = States::INITIAL;
bool handled;

void setup()
{
    myservo.attach(3);

    pinMode(MOTOR1, OUTPUT);
    pinMode(MOTOR2, OUTPUT);
    
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_END, INPUT_PULLUP);
    pinMode(BUTTON_RETURNED, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON_END), stop_motor, FALLING);

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

        digitalWrite(MOTOR1, LOW);
        digitalWrite(MOTOR2, HIGH);
        break;
    
    case States::TESTING:
        Serial.println("TESTING");
        delay(10000);
        break;

    case States::FIND_START:
        if (digitalRead(BUTTON_RETURNED) == LOW )
        {
            state = States::IDLE;
            Serial.println("IDLE");

            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, HIGH);
        }
        break;
    
    case States::IDLE:
        if (digitalRead(BUTTON_START) == LOW)
        {
            mario_racing();
            state = States::MOVE_TO_END;
            Serial.println("MOVE TO END");

            // activate motor ccw
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, LOW);
            crazyfrog();
        } else
          {
            // Stop motor
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, HIGH);
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

        // activate motor cw
        digitalWrite(MOTOR1, LOW);
        digitalWrite(MOTOR2, HIGH);

        myservo.write(INIT_POS);
        break;

    case States::RETURN_TO_START:
        if (digitalRead(BUTTON_RETURNED) == LOW)
        {
            state = States::IDLE;
            Serial.println("IDLE");
            
            // Stop motor
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, HIGH);
        }
        break;

    default:
        break;
    }
}

void stop_motor()
{
    if (state == States::MOVE_TO_END && !handled)
    {
        // Stop motor
        digitalWrite(MOTOR1, HIGH);
        digitalWrite(MOTOR2, HIGH);
        Serial.println("MOTOR STOP");
        handled = true;
    }    
}