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

def square(side_len):
    '''
    Draws a square with side lengths equal to the value of side_len
    '''
    color('black','lightpink')
    begin_fill()
    
    for i in range(4):
        fd(side_len)
        lt(90)
    end_fill()
    return

def roof(side_len):
    '''
    Draws a a triangle with a fixed side length
    '''
    color('black','red')
    begin_fill()
    rt(-45)
    fd(side_len)
    rt(90)
    fd(side_len)
    rt(135)
    fd(150)
    end_fill()

    return
def flower():
    '''
    Calls square() to create a flower pattern
    '''
    jump(150,-50)
    for i in range(15):
        rt(30)
        square(side_len=50)
        pass
    jump(150,-50)
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
    title("A Sunny Day :)")
    speed('fastest')
    bgcolor('lightblue')
    jump(-300,0)
    square(side_len=150)
    jump(-300,150)
    roof(side_len=106)
    jump(-200,0)
    rt(90)
    square(side_len=75)
    flower()
    sunshine()
    return
main()
