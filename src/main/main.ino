#include <Servo.h>

// Pins
#define LED2 13
#define MOTOR1 4
#define MOTOR2 5
#define BUTTON_START 9
#define BUTTON_END 8
#define BUTTON_RETURNED 7

// Waiting periods
#define WAIT 100
#define TIME_TO_DELIVER 3000

// Positioning
#define INIT_POS 0
#define DEPLOY_POS 90

Servo myservo;

void setup()
{
    myservo.attach(3);

    pinMode(LED2, OUTPUT);
    pinMode(MOTOR1, OUTPUT);
    pinMode(MOTOR2, OUTPUT);
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
    ERROR,
    TESTING
};

States state = States::INITIAL;

int pos = INIT_POS;

void loop()
{
    switch (state)
    {
    case States::INITIAL:
        Serial.println("\nInitialising");
        myservo.write(pos);
        delay(15);
        state = States::IDLE;
        Serial.println("Running");
        break;
    
    case States::TESTING:
                       
        break;
    
    case States::IDLE:
        if (digitalRead(BUTTON_START) == LOW)
        {
            state = States::MOVE_TO_END;

            // activate motor cw
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, LOW);
        } else
          {
            // Stop motor
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, HIGH);
          }
        break;
    case States::MOVE_TO_END:
        if (digitalRead(BUTTON_END) == LOW)
        {
            state = States::DELIVER_BALL;

            // Stop motor
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, HIGH);

            delay(1000);
            // eject
            pos = DEPLOY_POS;
            myservo.write(pos);
        }        
        break;
    case States::DELIVER_BALL:
        delay(TIME_TO_DELIVER);

        state = States::RETURN_TO_START;

        // activate motor ccw
        digitalWrite(MOTOR1, LOW);
        digitalWrite(MOTOR2, HIGH);

        pos = INIT_POS;
        myservo.write(pos);
        break;
    case States::RETURN_TO_START:
        if (digitalRead(BUTTON_RETURNED) == LOW)
        {
            state = States::IDLE;

            // Stop motor
            digitalWrite(MOTOR1, HIGH);
            digitalWrite(MOTOR2, HIGH);
        }
        break;
    case ERROR:
        break;
    default:
        break;
    }
}