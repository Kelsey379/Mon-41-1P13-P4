//Note: this is old code, no longer used for final prototype
//sample code to test button functionality - will be refactored and integrated with sensitivity_control.ino
const int buttonPin1 = 6;
const int ledPin1 = 13;  
const int buttonPin2 = 5;
const int ledPin2 = 12;  
const int buttonPin3 = 4;
const int ledPin3 = 11;  
const int buttonPin4 = 3;
const int ledPin4 = 10;  

int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;
int joyX A5;
int joyY A3;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(buttonPin4, INPUT);
}

void loop() {
checkButtonState();
checkJoyPosition(joyX, joyY);

}

//function to check position of joystick
int checkJoyPosition(joyX, joyY){
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);

  Serial.println(xValue);
  Serial.println(yValue);
  return xValue, yValue;
}

//buttonState returns int 1 or 0 based on high or low
void checkButtonState() {

  state1 = digitalRead(buttonPin1);
  state2 = digitalRead(buttonPin2);
  state3 = digitalRead(buttonPin3);
  state4 = digitalRead(buttonPin4);

  if (state1 == HIGH) {
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin1, HIGH);

  } else if(state2 = HIGH){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin2, HIGH);

  } else if(state3 == HIGH){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin3, HIGH);

  } else if(state4 == HIGH){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, HIGH);

  } else {
    Serial.write("No buttons are active at the moment")
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
  }
}
