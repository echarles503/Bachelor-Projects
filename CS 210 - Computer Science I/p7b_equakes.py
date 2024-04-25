'''
Title: Earthquake Watch
Author: Elias Charles
Credits: n/a

This program reports on the mean median and mode of Earthquake data near Eugene, OR.
'''
import doctest

def equake_readf(fname):
    '''(fname:file)->list

    Reads file and compiles data into list
    '''

    with open(fname,'r') as myf:
        magnitudes = []
        for line in myf:
            data = line.strip()
            mag = data[4]
            equakeList.append(mag)
        myf.close()   
        
    return magnitudes

equake_readf('p7b-equakes50f.txt')
