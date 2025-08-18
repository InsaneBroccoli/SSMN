#include <Servo.h>

#define LED2 13
#define MOTOR 5
#define WAIT 100
#define TIME_TO_DELIVER 1000
#define BUTTON_START 9
#define BUTTON_END 8
#define BUTTON_RETURNED 7
#define INIT_POS 0
#define DEPLOY_POS 90

Servo myservo;

void setup()
{
    myservo.attach(3);

    pinMode(LED2, OUTPUT);
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_END, INPUT_PULLUP);
    pinMode(BUTTON_RETURNED, INPUT_PULLUP);
    Serial.begin(9600);
}

enum States {
    INITIAL,
    IDLE,
    MOVE_TO_END,
    RETURN_TO_START,
    DELIVER_BALL,
    TESTING
};

States state = States::INITIAL;


void loop()
{
    switch (state)
    {
    case States::INITIAL:
        Serial.println("\nInitialising");

        int pos = INIT_POS;
        myservo.write(pos);
        state = States::IDLE;
        Serial.println("Running");
        break;
    
    case States::TESTING:
        digitalWrite(LED2, LOW);
        delay(WAIT);
        digitalWrite(LED2, HIGH);
        delay(WAIT);
        break;
    
    case States::IDLE:
        if (digitalRead(BUTTON_START) == LOW)
        {
            state = States::MOVE_TO_END;
            Serial.println("Move to end");
            digitalWrite(MOTOR, HIGH);
        } else
        {
            digitalWrite(MOTOR, LOW);
            delay(WAIT);
            digitalWrite(MOTOR, HIGH);
            delay(WAIT);
        }        
    break;
    case States::MOVE_TO_END:
        if (digitalRead(BUTTON_END) == LOW)
        {
            state = States::DELIVER_BALL;
            Serial.println("Deliver Ball");
            pos = DEPLOY_POS;
            myservo.write(pos);
            digitalWrite(MOTOR, LOW);
        }        
    break;
    case States::DELIVER_BALL:
        delay(TIME_TO_DELIVER);

        state = States::RETURN_TO_START;
        pos = INIT_POS;
        myservo.write(pos);
        Serial.println("Return to start");
    break;
    case States::RETURN_TO_START:
        if (digitalRead(BUTTON_RETURNED) == LOW)
        {
            state = States::IDLE;
            Serial.println("Idle");
        }
    break;
    default:
        break;
    }
}