'''
Title: File Concordance
Author: Elias Charles
Credits: n/a

This file locates the line number for each word in a given text file.
'''
import string



def fconcordance(fname):
    '''(fname: str)
    
    Locates the line number for every word in a given text file
    '''
    myDict = {}
    linenum = 0
    exclude = set(string.punctuation)
    
    with open(fname,'r') as f:
        for line in f:
            line = line.strip()
            line = line.lower()
            line = line.split()
            linenum += 1
            for word in line:
                word = word.strip()
                word = word.lower()
                word = ''.join(ch for ch in word if ch not in exclude)
            
            if word not in myDict:
                myDict[word] = []
            myDict[word].append(linenum)
    for key in sorted(myDict):
        print(f'{key} occurs in lines: {myDict[key]}')

    return

def main():
    '''

    Top level function that calls fconcordance with a given file name
    '''
    fname = 'tinyf.txt'
    fconcordance(fname)

    return

main()
