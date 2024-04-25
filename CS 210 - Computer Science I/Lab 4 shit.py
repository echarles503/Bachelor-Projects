def double_your_num():
    x = input('Enter an integer between 1 and 10: ')
    x = int(x)
    assert isinstance (x, int)
    assert x > 0 and x <= 10
    x = x*2
    print(x)
    return
double_your_num()
