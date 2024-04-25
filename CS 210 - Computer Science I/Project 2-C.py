'''
Title: Project 2-C - Better Art Show 

Author: Elias Charles

Credits: python.org
'''
from turtle import *

def jump(x, y):
    '''
    move turtle without leaving a track to position (x,y).
    like turtle setpos but pen is always up for the move
    and put down after

    >>> jump(0,0)
    [return turtle to center]
    '''
    pu()
    setpos(x,y)
    pd()

    return

def art_show_main():
    '''
    draw a picture of a house and sun and flower
    
    calls: house, sunshine, flower

    >>> art_show_main()
    '''
    reset()
    clear() 
    title("A Sunny Day")
    speed('fastest')
    bgcolor('lightblue')
    #hideturtle()
    
    # after each function is written, uncomment the line
    # that calls the function
    #house()
    #sunshine()
    
    square()
    roof()
    door()
    flower()
    return

def square():
    color('black','white')
    jump(-300,0)

    begin_fill()
    for i in range(4):
        fd(150)
        lt(90)
        
    end_fill()
    
    return
def roof():
    color('black','red')
    jump(-300,150)
    begin_fill()
    rt(-45)
    fd(106)
    rt(90)
    fd(106)
    rt(135)
    fd(150)
    end_fill()

    return

def door():
    color('black','yellow')
    jump(-200,0)
    rt(90)
    begin_fill()
    for i in range(2):
        fd(75)
        lt(90)
        fd(50)
        lt(90)
    end_fill()

def flower():
    color('green')
    jump(-50,-200)
    rt(45)
    fd(200)
    color('blue', 'yellow')
    begin_fill()
    while True:
        forward(200)
        left(170)
        if abs(pos()) < 1:
            break
    end_fill()
    pu()
    
    return


def sunshine():
    
    color('yellow')
    jump(200,200)
    r = 5
    for i in range(100):
        pd()
        circle(r + i, 45)
        pass
    jump(200,200)
    for i in range(9): 
        jump(200,200)
        lt(45)
        pu()
        fd(100) 
        pd() 
        fd(100) 
        pass
    jump(0,0)

    return      
art_show_main()


