#import mouse, sys
#import time 
import serial
#from pynput.keyboard import Key, Controller
from pynput import mouse, keyboard
from pynput.keyboard import Key
from pynput.mouse import Button



#Visualizer 
import pygame
scale = 1/2 #changes ratio of pot number to pixels on screen
divisions = 10 #changes numbe of grid lines that appear
pygame.init()
screen = pygame.display.set_mode((1024*scale,1024*scale))

def renderDisplay(x,y,scale,divisions):
    center = 1024*scale/2

    running = True

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill((200,200,255)) #clear screen
    pygame.draw.rect(screen,(200,255,200),(center-100*scale,0,200*scale,1024*scale))
    pygame.draw.rect(screen,(200,255,200),(0,center-100*scale,1024*scale,200*scale))

    #pygame.draw.rect(screen, "white",(center-300*scale,center-300*scale,300*scale*2,300*scale*2))
    pygame.draw.circle(screen,"white",(center,center),300*scale)
    
    pygame.draw.circle(screen,"red",(x * scale,(1024-y)*scale),5) #Draw joystick position
                                    #swap coordinates on y
    #Draw grid
    for i in range(0, divisions):
        divisionSize = 1024/divisions*scale
        
        if i == divisions/2:
            #Make center lines black
            pygame.draw.line(screen, "black",(0,divisionSize*i),(1024*scale, divisionSize*i), 1)
            pygame.draw.line(screen, "black",(divisionSize*i, 0),(divisionSize*i,1024*scale), 1)

        else:
            pygame.draw.line(screen, "grey",(0,divisionSize*i),(1024*scale, divisionSize*i), 1)
            pygame.draw.line(screen, "grey",(divisionSize*i, 0),(divisionSize*i,1024*scale), 1)

    
    #Render text
    font = pygame.font.Font(pygame.font.get_default_font(), 14)
    text = font.render(f"{1024/divisions} units per division ({x},{y})", True, "Black")
    screen.blit(text, (0,0))

    pygame.display.flip()
    return running


ser = serial.Serial('COM11')

keyboardController = keyboard.Controller()
mouseController = mouse.Controller()

ser.flushInput()

running = True
while running:
    ser_bytes = ser.readline()
    decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("ascii"))
    #print(decoded_bytes)
    

    dataType, data = decoded_bytes.split("\t")

    if dataType == "POS":
        xPos, yPos = data.split(":")
        xPos, yPos = int(xPos),int(yPos)
        running = renderDisplay(xPos,yPos,scale,divisions)

    elif dataType == "DATA":
        mode, value = data.split("$")
        
        if mode == "0": #keyboard mode
            if value != "/":
                try:
                    if value == "4":
                        #print("backspace")
                        keyboardController.press(Key.backspace)
                        keyboardController.release(Key.backspace)
                    elif value == "3":
                        #print("enter")
                        keyboardController.press(Key.enter)
                        keyboardController.release(Key.enter)
                    else:
                        keyboardController.press(value)
                        keyboardController.release(value)
                        #print("typing letters wow")
                except:
                    print("Error of some type")

        elif mode == "1":

            #Assign x, y, and z positions
            (x,y,mousePress)=value.split(":")

            (x,y)=(int(x),int(y))
            
            
            #Move the cursor 
            mouseController.move(x,-y)
            
            #If the joystick is pressed, left click
            if ('1' in mousePress):
                mouseController.click(Button.left)
