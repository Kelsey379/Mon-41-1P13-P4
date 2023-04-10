import pygame
import serial
import time 

# Init serial
#ArduinoSerial=serial.Serial('COM5',9600)  
time.sleep(1)

scale = 1/2 #changes ratio of pot number to pixels on screen
divisions = 10 #changes numbe of grid lines that appear

# Init pygame,5
pygame.init()
screen = pygame.display.set_mode((1024*scale,1024*scale))


running = True


def renderDisplay(x,y,scale,divisions):
    center = 1024*scale/2

    screen.fill((200,200,255)) #clear screen
    pygame.draw.rect(screen,(200,255,200),(center-100*scale,0,200*scale,1024*scale))
    pygame.draw.rect(screen,(200,255,200),(0,center-100*scale,1024*scale,200*scale))

    pygame.draw.circle(screen,"white",(center,center),300*scale)
    
    pygame.draw.circle(screen,"red",(x * scale,y*scale),5) #Draw joystick position

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

while running:

    try:
        #data=str(ArduinoSerial.readline().decode('ascii'))
        data = "512\t512" #Testing without physical hardware
        
        #Assign x, y, and z positions
        (x , y)=data.split("\t")
        x,y = int(x), int(y) #Cast to int

        

    except Exception as exception:
        print(f"Exception {exception.args}")

    renderDisplay(x,y,scale,divisions)
    
    
