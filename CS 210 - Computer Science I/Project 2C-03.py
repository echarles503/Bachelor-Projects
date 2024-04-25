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

def square(side_len,fillcolor):
    '''
    Draws a square with side lengths equal to the value of side_len
    '''

    color(fillcolor)
    begin_fill()
    for i in range(4):
        fd(side_len)
        lt(90)
    end_fill()
    return

def roof(side_len,fillcolor):
    '''
    Draws a a triangle with a fixed side length
    '''
    color(fillcolor)
    begin_fill()
    rt(-45)
    fd(side_len)
    rt(90)
    fd(side_len)
    rt(135)
    fd(150)
    end_fill()

    return
def flower(fillcolor='lightpink'):
    '''
    Calls square() to create a flower pattern
    '''
    jump(150,-50)
    for i in range(15):
        rt(30)
        square(side_len=50,fillcolor='blue')
        pass
    jump(150,-50)
    color('green')
    pensize(4)
    lt(90)
    bk(200)
    return

def sunshine(ray_len=75):
    '''
    Borrowed code from a doc.python example to create a spiral sun with rays
    '''
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
        fd(ray_len) 
        pass
    jump(0,0)

    return      
def main():
    reset()
    clear()
    hideturtle()
    title("A Sunny Day :)")
    speed('fastest')
    bgcolor('lightblue')
    jump(-300,0)
    square(150,'white')
    jump(-300,150)
    roof(106,'red')
    jump(-200,0)
    rt(90)
    square(75,'green')
    flower()
    sunshine()
    return
main()


#This program is not fully optimized to the fullest extent it could be
#I could not figure out how to define poly() in the final part of the instructions.
