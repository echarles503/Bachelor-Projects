'''
Title: Inspecting Files
Author: Elias Charles
Credits: n/a

This program counts several aspects of a file, including word count, line count, and # of characters.
'''
import string
#from itertools.chain import chain
from p6a_data_analysis import frequencyTable

def fcounts(fname):
    with open(fname,'r') as myf:
        contents = []
        word_ct = 0
        char_ct = 0
        for line in myf:
            words = line.split()
            word_ct += len(words)   #splits by individual word
            words = ''.join(words)  #conjoins words without spacing
            char_ct += len(words)
            contents.append(words)  
        line_ct = len(contents)     #  no. of nested lists equals line count
        #print(word_ct)
        #print(line_ct)
        #print(char_ct)
    with open(fname,'r') as myf:
        rawList = []
        space_ct = 0
        for line in myf:
            char = line.split()
            char = ' '.join(char)
            #print(char)
            space_ct += len(char)   #counts words with spacing
            freqW = char.split() 
            rawList.append(freqW)   #prepare nested list 
        #print(space_ct)

    tableList = []
    while rawList:
        tableList.extend(rawList.pop(0)) #consumes original list removing nested lists in the process
    #print(tableList)
    masterList = [line_ct, word_ct, char_ct, space_ct, tableList]
    return masterList

def report_fcount(words,lines,chars,spaces,tableList,fname):
    '''(words:int,lines:int,chars:int,spaces:int,tableList:list,fname:str) -> None

    Reports the quantitative analysis of the document
    '''
    print(f'in file {fname}:')
    print(f'the number of lines is: {lines}')
    print(f'the number of words is: {words}')
    print(f'the number of characters (no spaces) is: {chars}')
    print(f'the number of characters (with spaces) is: {spaces}')
    print(f'\nWord occurrences in file {fname}:\n')
    frequencyTable(tableList)

    
def main():
    fname = 'test-doc.txt'
    masterList = fcounts(fname)
    tableList = masterList[4]
    lines = masterList[1]
    chars = masterList[2]
    spaces = masterList[3]
    words = masterList[0]
    report_fcount(words,lines,chars,spaces,tableList,fname)
    return

main()
