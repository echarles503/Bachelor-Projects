from turtle import *
color('blue', 'orange')
begin_fill()
while True:
    forward(150)
    left(144)
    if abs(pos()) < 1:
        break
end_fill()
done()
