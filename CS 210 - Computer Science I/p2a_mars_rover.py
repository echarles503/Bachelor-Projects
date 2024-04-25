'''
Title: Project 2-a - Mars rover 

Author: Elias Charles

Credits: N/A

'''
from turtle import *
from random import randint



def rover_loc():
    '''
    returns a random integer for location between -275 and 275
    '''
    xpos = randint(-275,275)
    ypos = randint(-275,275)
    pu()
    fd(xpos)
    lt(90)
    fd(ypos)
    dot()
    print(xpos, ypos)
    setpos(0,0)
    
    return xpos, ypos

def water_content():
    '''
    returns a random integer for water pressure between 1 and 290(ppm)
    '''
    water = randint(1,290)
    print(water)
    return water

def temperature():
    '''
    returns a random integer for temperature between -178 and 1 (degrees fahrenheit)
    '''
    temp = randint(-178,1)
    print(temp)
    return temp


def mars_explore():
    rover_loc()
    water_content()
    temperature()
    return

def mars_explore_main():
    '''
    main function for mars_explore:
        set up print and graphical output
        then call mars_explore repeatedly

    >>> mars_explore_main()
    ''' 
    print('xpos', '\t',
          'ypos','\t',
          'water', '\t',
          'temp')               # label for print output - use tab

    # set up graphical output
    reset()
    screensize(-500,500)
    title('Mars Rover')
    display_color = 'blue'
    pencolor(display_color)
    dot(10, display_color)      # mark the rover's starting position

    # explore Mars

    for i in range(20):         # pass is legal Python but it is
        pass                    # just a placeholder. Delete pass
        mars_explore()         # and uncomment mars_explore()
                                # after mars_explore is written.
    return
mars_explore_main()
