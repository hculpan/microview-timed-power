#include <MicroView.h>		// include MicroView library

int buttonPin = A0;     	// push button pin
int buttonState = 0;		// variable to store the pushbutton status

int longButtonPressInSeconds = 2;

signed long countdownSeconds = 0;
unsigned long countdownStart = 0;
unsigned long buttonPressStart = 0;
int buttonPressed = true;
int longButtonPressed = false;
char outputBuffer[50];

void setup() {
        uView.begin();			// start MicroView
      	uView.clear(PAGE);		// clear page
        uView.setFontType(1);
        pinMode(buttonPin, INPUT);  	// initialize the pushbutton pin as an input
	digitalWrite(buttonPin, HIGH); 	// set Internal pull-up
        countdownExpired();
        Serial.begin(9600);
}

void countdownExpired() {
  countdownSeconds = 0;
  countdownStart = 0;
  uView.clear(PAGE);
  uView.display();
  
  // Put Power On here
}

void countdownStarted() {
  // Put power off here
}

void loop() {
	buttonState = digitalRead(buttonPin);	// read the state of the pushbutton value

        // Pressing button
        if (buttonState == HIGH && !buttonPressed) {
          // Pressed for the first time
          if (countdownSeconds == 0) {
            countdownStart = millis()/1000;
            countdownStarted();
          }
          countdownSeconds += 15 * 60;
          // One-hour max
          if (countdownSeconds > 3600) {
            countdownStart = millis()/1000;
            countdownSeconds = 3600;
          }
          buttonPressed = true;
        } else if (buttonState == LOW && buttonPressed) { // depressed button
          buttonPressStart = 0;
          buttonPressed = false;
        } else if (buttonState == LOW && buttonPressStart == 0) {
          buttonPressStart = millis()/1000;
        } else if (buttonState == LOW) {
          signed long buttonLength = longButtonPressInSeconds - ((millis()/1000) - buttonPressStart);
          longButtonPressed = (buttonLength < 0);
        }

        if (longButtonPressed) {
          countdownExpired();
        } else if (countdownSeconds != 0) {
          signed long remaining = countdownSeconds - ((millis()/1000) - countdownStart);
          if (remaining < 1) {
            countdownExpired();
          } else {
            uView.clear(PAGE);
            uView.setCursor(1,1);
            sprintf(outputBuffer,"\n%3d:%02d", (int)(remaining/60), (int)(remaining%60));
            uView.print(outputBuffer);
            uView.display();
          }
        }
}
