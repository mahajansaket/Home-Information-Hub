#define timeSeconds 10

// Set GPIOs for LED and PIR Motion Sensor
const int led = 27;
const int motionSensor = 26;
const int tempPin = 25; //analog input pin constant
const int buzzerPin = 22;
int tempVal; // temperature sensor raw readings

float volts; // variable for storing voltage

float temp; // actual temperature variable

const int a6 = 1760;
const int e7 = 2637;
const int c7 = 2093;
const int g7 = 3136;
const int g6 = 1568;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement()
{
    Serial.println("MOTION DETECTED!!!");
    digitalWrite(led, HIGH);
    pinMode(buzzerPin, OUTPUT);
    startTimer = true;
    lastTrigger = millis();
}

void setup()
{
    // put your setup code here, to run once:
    // Serial port for debugging purposes
    Serial.begin(115200);

    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(motionSensor, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

    // Set LED to LOW
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    // Current time
    now = millis();
    // Turn off the LED after the number of seconds defined in the timeSeconds variable
    if (startTimer && (now - lastTrigger > (timeSeconds * 1000)))
    {
        Serial.println("Motion stopped...");
        //pinMode(buzzerPin, OUTPUT);
        digitalWrite(led, LOW);
        startTimer = false;
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Current time
    now = millis();
    // Turn off the LED after the number of seconds defined in the timeSeconds variable
    if (startTimer && (now - lastTrigger > (timeSeconds * 1000)))
    {
        Serial.println("Motion stopped...");
        digitalWrite(led, LOW);
        alertSound();
        delay(3000);

        //read the temp sensor and store it in tempVal
        tempVal = analogRead(tempPin);
        volts = tempVal / 1023.0; // normalize by the maximum temperature raw reading range

        temp = (volts - 0.5) * 100; //calculate temperature celsius from voltage as per the equation found on the sensor spec sheet.
        Serial.print(tempVal);
        Serial.print("\n");
        Serial.print(" Temperature is:   ");
        // print out the following string to the serial monitor
        Serial.print(temp); // in the same line print the temperature
        Serial.println(" degrees C");
        Serial.print("\n");
        delay(100); // wait for 1 second or 1000 milliseconds before taking the next reading.

        startTimer = false;
    }
}
void beep(int tone, int duration)
{
    for (long i = 0; i < duration * 900L; i += tone * 1)
    {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(tone * (.50));
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(tone * (.50));
    }
    delay(30);
}
void alertSound()
{
    beep(e7, 150);
    delay(150);
    beep(a6, 150);
    delay(150);
    beep(c7, 150);
    delay(150);
    beep(g7, 150);
    delay(450);
    beep(g6, 150);
    delay(450);
    beep(c7, 150);
}
