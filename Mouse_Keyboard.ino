const int switchPin = 7;

//Keyboard Setup:
#define joyX A0
#define joyY A1

int CENTER_DEADZONE = 300;
int PRIMARY_AXIS_RANGE = 100;
int HOLD_TIME = 200; //how long a letter needs to be hield inorder to be typed (in ms)
int REPEAT_TIME = 400;

// initialize button pins
const int buttonPin1 = 3;
const int buttonPin2 = 4;
const int buttonPin3 = 5;
const int buttonPin4 = 6;

// initialize button states
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

// Define a variable to keep track of the toggle state
//bool toggleState1 = false;
//bool toggleState2 = false;
//bool toggleState3 = false;
//bool toggleState4 = false;
int activeButton;

//setup the mouse:
int prev_state = 0;
//int mouseButton1 = 0;
//bool coords = true;

char charcter;
String mouseState;
int xValue;
int yValue;
int switchState;
int holdTime;
bool waiting = false;
long waitStartTime = 0;
char previousCharcter = '/';

void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT_PULLUP);
  // initialize the buttons as input
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  //setup the joystick
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
}

void loop() {
  // Read the state of the switch
  switchState = digitalRead(switchPin);
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);

  // mode \t data from mode \t joystickX:joystickY

  // Call the appropriate function based on the switch state

  sendPosition(xValue, yValue);

  if (switchState == LOW) {
    if (waiting == false) { //if we are not waiting for a new charcter, get a new one
      charcter = keyboard(xValue, yValue);
      waiting = true;
      waitStartTime = millis();

      if (charcter == previousCharcter) {
        holdTime = REPEAT_TIME; //If its a new char then use REPEAT_TIME
      } else {
        holdTime = HOLD_TIME; //If its the same char then use HOLD_TIME
      }
      
    } else {
      // waiting to see if charcter changes

      if (millis() >= waitStartTime + holdTime) {
        // If we have waited the holding time
        sendData(switchState, String(charcter)); //Send the char and reset
        waiting = false;
        previousCharcter = charcter;
      }

      if (keyboard(xValue, yValue) != charcter) {
        //if the char is not heild the whole time, reset
        waiting = false;
      }
    }

  } else {
    //Send mouse movement amounts
    mouseState = mouse(xValue, yValue);
    sendData(switchState, String(mouseState));
  }



  //  delay(100); // wait a short time before checking the switch again
}

void sendData(int switchState, String data) {
  Serial.println("DATA\t" + String(switchState) + "$" + data);
}

void sendPosition(int xValue, int yValue) {
  Serial.println("POS\t" + String(xValue) + ":" + String(yValue));
}

char mapButton(int xValue, int yValue, int activeButton) {

  //define lists for each position
  char up[4] = {'H', 'J', 'I', 'C'};
  char down[4] = {'L', 'Y', 'T', '?'};
  char left[4] = {'O', 'B', 'U', 'M'};
  char right[4] = {'E', 'Q', 'S', 'F'};
  //define lists for the diagonals
  char tr[4] = {'R', 'Z', 'N', 'G'};
  char tl[4] = {'W', 'X', 'U', 'K'};
  char br[4] = {' ', '!', '.', '3'};
  char bl[4] = {'D', 'V', '4', 'P'};

  //Center is 512

  //Check center deadzone

  if ((xValue > (512 - CENTER_DEADZONE) && xValue < (512 + CENTER_DEADZONE)) && (yValue > (512 - CENTER_DEADZONE) && yValue < (512 + CENTER_DEADZONE))) {
    return '/';
  }


  if (xValue > (512 - PRIMARY_AXIS_RANGE) && xValue < (512 + PRIMARY_AXIS_RANGE)) {
    //are somwhere in vertical axis
    if (yValue > 512) {
      // top
      return up[activeButton];
    } else {
      //bottom
      return down[activeButton];
    }
  }

  if (yValue > (512 - PRIMARY_AXIS_RANGE) && yValue < (512 + PRIMARY_AXIS_RANGE)) {
    //are somwhere in horizontal axis
    if (xValue > 512) {
      // right
      return right[activeButton];
    } else {
      //left
      return left[activeButton];
    }
  }

  if (xValue > 512 && yValue > 512) {
    //top right
    return tr[activeButton];
  } else if (xValue > 512 && yValue < 512) {
    //bottom right
    return br[activeButton];
  } else if (xValue < 512 && yValue > 512) {
    //top left
    return tl[activeButton];
  } else if (xValue < 512 && yValue < 512) {
    //bottom left
    return bl[activeButton];
  }
}

char keyboard(int xValue, int yValue) {

  // read button states
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

  //Serial.println(String(buttonState1) + ':' +String(buttonState2) + ':' +String(buttonState3) + ':' +String(buttonState4)+ ':' +String(activeButton));

  // Update the active button based on what button was pressed
  if (buttonState1 == HIGH) {
    activeButton = 0;
  } else if (buttonState2 == HIGH) {
    activeButton = 1;
  } else if (buttonState3 == HIGH) {
    activeButton = 2;
  } else if (buttonState4 == HIGH) {
    activeButton = 3;
  }

  char key = mapButton(xValue, yValue, activeButton);

  return key;

}

//define mouse function

String mouse(int x, int y) {

  int z = 0, xpos = 0, ypos = 0;
  //change sensitivity value based on preferences
  int sensitivity = 3;
  //map up, down, left, and right positions (in that order)
  if (x >= 550)
    xpos = map(x, 550, 1023, 0, sensitivity);
  if (x <= 450)
    xpos = map(x, 450, 0, 0, -sensitivity);
  if (y >= 550)
    ypos = map(y, 550, 1023, 0, sensitivity);
  if (y <= 450)
    ypos = map(y, 450, 0, 0, -sensitivity);
  int curr_state = digitalRead(9);
  //check if SW pressed
  if (curr_state == 1 && prev_state == 0)
    z = 1;
  else
    z = 0;


  //print the positions (for debugging, can remove later if needed)
  //if ((xpos != 0 or ypos != 0 or z == 1) && coords) {
  //  Serial.print(coords);
  //    Serial.print(xpos);
  //    Serial.print(":");
  //    Serial.print(ypos);
  //    Serial.print(":");
  //    Serial.println(z);
  //}
  prev_state = curr_state;

  return String(xpos) + ':' + String(ypos) + ':' + z;

  //  delay(10);
}
