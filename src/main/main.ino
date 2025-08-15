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
    LEDS
};

States state = States::INITIAL;


void loop()
{
    switch (state)
    {
    case States::INITIAL:
        Serial.println("\nInitialising");
        state = States::LEDS;
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