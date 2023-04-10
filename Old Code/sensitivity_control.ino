#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
}

// Set the initial state of the switch to 1 (on)
// This will make sure that the program does not assume the joystick to be moving initially
int switch_state = 1;

void loop()
{
  int x = analogRead(A0);
  int y = analogRead(A1);

  // Set initial positions
  int z = 0, x_pos = 0, y_pos = 0;

  // Set sensitivity level - this can be adjusted if needed
  int sensitivity_level = 10;

  // Add code to map up, down, right, and left movement (in this order)
  if (x >= 550)
    x_pos = map(x, 550, 1023, 0, sensitivity_level);
  if (x <= 450)
    x_pos = map(x, 450, 0, 0, -sensitivity_level);
  if (y >= 550)
    y_pos = map(y, 550, 1023, 0, sensitivity_level);
  if (y <= 450)
    y_pos = map(y, 450, 0, 0, -sensitivity_level);

  int curr_state = digitalRead(2);

  // Check when button is pressed
  if (curr_state == 0 && switch_state == 1)
    z = 1;
  else
    z = 0;

  // Print updated position details when joystick moved - will remove for final but useful for debugging
  if (x_pos != 0 || y_pos != 0 || z == 1)
  {
    Serial.print(x_pos);
    Serial.print(":");
    Serial.print(y_pos);
    Serial.print(":");
    Serial.println(z);
  }

  switch_state = curr_state;

  // Add 10ms delay
  delay(10);
}


// //TODO: double-check and debug map values, add button functionality, refactor button and jostick code 
// #include<Arduino.h>
// #include<HardwareSerial.h>

// void setup()   
// {
//   Serial.begin(9600);
//   pinMode(2,INPUT);
//   digitalWrite(2,HIGH);

// }
// //set the initial state of the switch to 0 (off)
// //this will make sure that the program doe not assume the joystick to be moving initially
// int switch_state =0;

// void loop() {
//   int x=analogRead(A0);
//   int y=analogRead(A1);
//   //set initial positions 
//   int z=0,x_pos=0,y_pos=0;
//   //set sensitivity level - this can be adjusted if needed
//   int sensitivity_level=10;

// //add code to map up, down, right, and left movement (in this order)
//   if(x>=550)
//   x_pos=map(x,550,1023,0,sensitivity_level); 
//   if(x<=450)
//   x_pos=map(x,450,0,0,-sensitivity_level);   
//   if(y>=550)
//   y_pos=map(y,550,1023,0,sensitivity_level);  
//   if(y<=450)
//   y_pos=map(y,450,0,0,-sensitivity_level); 
//   int curr_state=digitalRead(2);

//   //check when SW is pressed
//    if(curr_state==1 && switch_state ==0)
//   z=1;
//   else
//    z=0;

//    //prints updated position details when jostick moved - will remove for final but useful for debugging
//   if(_!=0 or y_pos!=0 or z==1)
//    {
//   Serial.print(x_pos);
//   Serial.print(":");
//    Serial.print(y_pos);
//   Serial.print(":");
//   Serial.println(z);
//   }
//    switch_state =curr_state;
//    // add 10ms delay
//   delay(10);
// }
