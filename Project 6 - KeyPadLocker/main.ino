#include <LiquidCrystal_I2C.h>

#include <Keypad.h>

void beep();
void beepError();
void writeText(char key);
void writeSetPasswordText();

const int PAD_ROWNS = 4;
const int PAD_COLS = 4;

const int LCD_COLS = 16;
const int LCD_ROWNS = 2;

byte PAD_ROWN_PINS[PAD_ROWNS] = {9, 8, 7, 6};
byte PAD_COL_PINS[PAD_COLS] = {5, 4, 3, 2};

const int BUZZER_PIN = 12;

char padKeys[PAD_ROWNS][PAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


Keypad keypad = Keypad(makeKeymap(padKeys), PAD_ROWN_PINS, PAD_COL_PINS, PAD_ROWNS, PAD_COLS);
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWNS);

const int MAX_PASSWORD_LENGTH = 3;

int cursorPositionX = 0;
char passwordKeys[MAX_PASSWORD_LENGTH] = {};

void setup(){
    pinMode(BUZZER_PIN, OUTPUT);

    lcd.init();
    lcd.backlight();
    writeSetPasswordText();
}

void loop(){
    const char key = keypad.getKey();

    if(key){
        
        if(passwordKeys)

        
        
        if(key == '*'){

        }else if (key == '#'){

        }

        writeText(key);
    }

}

void writeSetPasswordText(){
    lcd.setCursor(0, 0);
    lcd.print("Zadejte heslo:");
}

void beep(){
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

void beepError(){
  beep();
  delay(100);
  beep();
}

void writeText(char key){


    if(cursorPositionX + 1 > MAX_PASSWORD_LENGTH){
        beepError(); 

        memset(passwordKeys, 0, MAX_PASSWORD_LENGTH);
        lcd.clear();
        writeSetPasswordText();

        cursorPositionX = 0;
    }else {
        lcd.setCursor(cursorPositionX, 1);
        lcd.print("*");

        beep();

        passwordKeys[cursorPositionX] = key;
        cursorPositionX++;
    }






}