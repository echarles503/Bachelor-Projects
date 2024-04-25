'''
Title: Project 1-A UO GUIDE

Author: Elias Charles

Credits: CIS 210
'''
from turtle import *

def uo_guide():
    '''
    Welcome to the UO!  Welcome to Computer Science!
    Guide students from the EMU Lawn to Deschutes Hall, home of the
    Computer Science Department, and then to Price Science Commons
    (Science Library), home of B004/A computer lab and study space.

    >>> uo_guide()
    '''
    # set the scene 
    reset()
    clear()
    title('Welcome to Computer Science at the University of Oregon!')
    color('purple')
    pensize(3)
    speed('slowest')
    bgpic('uo_campus_map.png')
    screensize(1195, 681)

    dot() #mark start of route on EMU East lawn

    lt(90)
    fd(50)
    rt(90)
    fd(50)
    lt(90)
    fd(25)
    dot()
  
    # guide to Deschutes
    pass
    color('green')
    lt(180)
    fd(25)
    rt(90)
    fd(100)
    rt(90)
    fd(50)
    rt(90)
    fd(10)
    dot()
    hideturtle()
    # guide to Price Science Commons
    pass
    
    return
def main():
    uo_guide()
    return
main()
