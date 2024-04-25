'''
Title: Zip to Bar
Author: Elias Charles
Credits: Aidan Hart

Encodes and decodes a barcode for a given zipcode
'''


from turtle import *

def ziptoBar(zipcode):
'''(zipcode: str) -> None

Encodes and prints a zipcode
'''
    aList = []
    numList = []
    
    num_0 = '11000'
    num_1 = '00011'
    num_2 = '00101'
    num_3 = '00110'
    num_4 = '01001'
    num_5 = '01010'
    num_6 = '01100'
    num_7 = '10001'
    num_8 = '10010'
    num_9 = '10100'
    

    pu()
    setpos(-125,0)
    pd()
    full_bar()
    
    for i in zipcode:
        numList.append(i)
    numList.append(check_digit(zipcode))
    
    for i in range(len(numList)):
        for item in numList[i]:
            if item == '9':
                aList.append(num_9)
            if item == '8':
                aList.append(num_8)
            if item == '7':
                aList.append(num_7)
            if item == '6':
                aList.append(num_6)
            if item == '5':
                aList.append(num_5)
            if item == '4':
                aList.append(num_4)
            if item == '3':
                aList.append(num_3)
            if item == '2':
                aList.append(num_2)
            if item == '1':
                aList.append(num_1)
            if item == '0':
                aList.append(num_0)
    print(numList)
   
    for i in range(len(aList)):
        for ch in aList[i]:
            print(ch)
            if ch == '0':
                half_bar()
            if ch == '1':
                full_bar()
    full_bar()
    return

def check_digit(n):
    '''(n:str) -> str
    Calculates the check digit that is located at the end of the barcode
    '''
    n = int(n)
    a = n // 10000
    b = (n % 10000) // 1000
    c = (n % 1000) // 100
    d = (n % 100) // 10
    e = (n % 10000) % 10
    sum_of_digits = a + b + c + d + e
    remainder = sum_of_digits % 10
    check_digit = 10 - remainder
    check_digit = str(check_digit)
    return check_digit

def full_bar():
    '''
    Turtle instructions for a full bar in the barcode
    '''
    lt(90)
    fd(50)
    pu()
    bk(50)
    rt(90)
    fd(10)
    pd()
    return

def half_bar():
    '''
    Turtle instructions for a half bar in the barcode
    '''
    lt(90)
    fd(25)
    pu()
    bk(25)
    rt(90)
    fd(10)
    pd()
    return


def main():
    '''
    Receives user input and calls to ziptoBar with the given input.
    '''
    ask = input("Enter a 5 digit zipcode:\n")
    zipcode = str(ask)
    ziptoBar(zipcode)
    
    return
main()


