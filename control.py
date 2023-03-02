#TODO: update click and position functionality
#Note: Testing using left click for now, will update with right click as well if needed

import mouse,time, serial, sys

mouse.FAILSAFE=False
ArduinoSerial=serial.Serial('com3',9600)
time.sleep(0.5) #edit sleep time if needed

while 1:
   data=str(ArduinoSerial.readline().decode('ascii'))    
   #split x, y, and z positions
   (x,y,z)=data.split(":") 
   #read current cursor position
   (X,Y)=mouse.get_position()
   #convert position to integer and update with the joystick position
   (x,y)=(int(x),int(y))
   mouse.move(X+x,Y-y)

   if '1' in z:
      mouse.click(button="left")    # clicks left button