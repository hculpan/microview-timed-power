#include <MicroView.h>		// include MicroView library

int buttonPin = A0;     	// push button pin
int buttonState = 0;		// variable to store the pushbutton status

signed long countdownSeconds = 0;
unsigned long countdownStart = 0;
int buttonPressed = true;
char outputBuffer[10];

void setup() {
        uView.begin();			// start MicroView
      	uView.clear(PAGE);		// clear page
        pinMode(buttonPin, INPUT);  	// initialize the pushbutton pin as an input
	digitalWrite(buttonPin, HIGH); 	// set Internal pull-up
        countdownExpired();
}

void countdownExpired() {
  countdownSeconds = 0;
  countdownStart = 0;
  uView.clear(PAGE);
  uView.display();
}

void loop() {
	buttonState = digitalRead(buttonPin);	// read the state of the pushbutton value

        // Pressing button
        if (buttonState == HIGH && !buttonPressed) {
          // Pressed for the first time
          if (countdownSeconds == 0) {
            countdownStart = millis()/1000;
          }
          countdownSeconds += 15 * 60;
          buttonPressed = true;
        } else if (buttonState == LOW && buttonPressed) { // depressed button
          buttonPressed = false;
        }
        
        if (countdownSeconds != 0) {
          signed long remaining = countdownSeconds - ((millis()/1000) - countdownStart);
          if (remaining < 1) {
            countdownExpired();
          } else {
            uView.clear(PAGE);
            uView.setCursor(0,0);
            sprintf(outputBuffer,"%2d:%02d", (int)(remaining/60), (int)(remaining%60));
            uView.print(outputBuffer);
            uView.display();
          }
        }
}
