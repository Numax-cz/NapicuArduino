const int LED_PIN = 13;
const int BUTTON_PIN = 2;

int button_state = 0;
int button_last_state = 0;

bool diode_on = false;

void setup(){
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop(){
    button_state = digitalRead(BUTTON_PIN);

    if (button_state != button_last_state) {
        if(button_state == HIGH) diode_on = !diode_on;
    }

    button_last_state = button_state;

    if(diode_on){
        digitalWrite(LED_PIN, HIGH);
    } else digitalWrite(LED_PIN, LOW);

    delay(100);
}