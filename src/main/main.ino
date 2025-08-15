#define LED1 5
#define LED2 13
#define WAIT 50

void setup()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    Serial.begin(9600);
}

enum States {
    INITIAL,
    IDLE,
    MOVE_TO_END,
    RETURN_TO_START,
    TESTING
};

States state = States::INITIAL;


void loop()
{
    switch (state)
    {
    case States::INITIAL:
        Serial.println("\nInitialising");
        state = States::TESTING;
        Serial.println("Running");
        break;
    
    case States::LEDS:
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        delay(WAIT);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        delay(WAIT);
        break;
    
    default:
        break;
    }
}