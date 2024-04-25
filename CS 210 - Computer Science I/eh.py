from turtle import *
def line_drawing(n=1):
    pencolor("#CD39C6")
    for ctr in range(5):
        fd(100)
        lt(90)
        fd(100)
        lt(90)
        fd(100)
        lt(90)
        fd(200)
    return
def jump(x,y):
    pu()
    setpos(x,y)
    pd()
def main():
    line_drawing()
    jump(400,100)
    line_drawing()
    return
main()
