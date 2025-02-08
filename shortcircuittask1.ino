const int RedLed = 2;
const int YellowLed = 4;
const int GreenLed = 7;
const int PushButton = 11;

int ButtonState;
bool pedestrianRequest = false;

unsigned long lastChangeTime = 0;
int state = 0;  // 0: Green, 1: Yellow, 2: Red
const long GreenTime = 5000;
const long YellowTime = 2000;
const long RedTime = 5000;

void setup() {
    pinMode(RedLed, OUTPUT);
    pinMode(YellowLed, OUTPUT);
    pinMode(GreenLed, OUTPUT);
    pinMode(PushButton, INPUT); // External 10kΩ resistor used
    Serial.begin(9600);
}

void loop() {
    // Read button input
    ButtonState = digitalRead(PushButton);
    Serial.println(ButtonState); 

    // If button is pressed, set pedestrian request flag
    if (ButtonState == LOW) {  // Button press pulls pin LOW (GND)
        pedestrianRequest = true;
    }

    unsigned long currentTime = millis();

    // Handle traffic light states
    if (pedestrianRequest == true) {
        // Immediately turn Red for pedestrians
        digitalWrite(GreenLed, LOW);
        digitalWrite(YellowLed, LOW);
        digitalWrite(RedLed, HIGH);
        
        if (currentTime - lastChangeTime >= RedTime) {
            pedestrianRequest = false; // Reset request after delay
            lastChangeTime = currentTime;
            state = 0; // Resume normal sequence
        }
    } 
    else {
        // Normal Traffic Light Cycle
        switch (state) {
            case 0: // Green
                digitalWrite(RedLed, LOW);
                digitalWrite(YellowLed, LOW);
                digitalWrite(GreenLed, HIGH);
                if (currentTime - lastChangeTime >= GreenTime) {
                    lastChangeTime = currentTime;
                    state = 1;
                }
                break;

            case 1: // Yellow
                digitalWrite(GreenLed, LOW);
                digitalWrite(YellowLed, HIGH);
                if (currentTime - lastChangeTime >= YellowTime) {
                    lastChangeTime = currentTime;
                    state = 2;
                }
                break;

            case 2: // Red
                digitalWrite(YellowLed, LOW);
                digitalWrite(RedLed, HIGH);
                if (currentTime - lastChangeTime >= RedTime) {
                    lastChangeTime = currentTime;
                    state = 0;
                }
                break;
        }
    }
}
