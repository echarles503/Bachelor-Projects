import doctest
debug = print

'''
Title: 'paritybit'
Author: Elias Charles
Credit: Jayden Ben, Austin, Aidan Hart

This script adds a parity bit to individual characters in a sequence of characters,
then checks for the parity bit in each character in a sequence of characters and reports
any errors
'''

def encode(letter):
    '''
    (letter: str) --> str

    encode will add a parity bit to the binary representation of a single letter string
    
    >>> encode('c') # binary representation of 'c' is 1100011
    '01100011'      # with parity bit – leftmost 0
    >>> encode('d') # binary representation of 'd' is 1100100
    '11100100'      # with parity bit – leftmost 1
    '''
    binary_version = bin(ord(letter))

    if parity(binary_version) == '1':
        return ('1' + binary_version[2:])
    else:
        return(binary_version[0:1] + binary_version[2:])

def parity(bitrep):
    '''
    (bitrep:str) --> str

    calculates the parity digit based 

    >>> parity('1100011') # binary representation of c
    '0' # parity bit is 0
    >>> parity('1100100') # binary representation of d
    '1' # parity bit is 1
    '''
    
    parity = str(bitrep)
    ct = 0
    for i in range(len(parity)):
        if parity[i] == '1':
            ct += 1
    #debug(ct)
    if ct % 2 == 0:
        return '0'
    else:
        return '1'
        

def decode(pletter):
    '''
    (byte: str) -> int

    converts a binary string back to its decimal representation if parity is even.
    if parity is odd it returns '*' to signify error.

    >>> int('1100011', 2)
    99
    '''
    check = parity(pletter)
    dec_version = int(pletter, 2)
    character = chr(dec_version)
    
    if check == '0':
        return character
    else:
        return '*'

def main():

    word = 'cat'
    for letter in word:
        print(decode(encode(letter)), end = '')
    print()
    return
