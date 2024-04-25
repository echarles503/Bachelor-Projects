'''
Title: Majors Report
Author: Elias Charles
Credit: n/a

This program analyzes the data of majors within CIS students. It reports the most popular major, the number of
unique majors, as well as a table of number of students per each major
'''
import doctest



def majors_readf(fname):
    '''(fname:file)->list

    Reads file and compiles data into list
    '''

    with open(fname,'r') as myf:
        majorsli = []
        for line in myf:
            majors = line.strip()
            majorsli.append(majors)
            
        del majorsli[0:2]
    return majorsli


def majors_analysis(majorsli):
    '''(majorsli: list) -> (list, int)

    Finds the mode of the list. Counts number of unique values in the list. Returns both of these values.
    '''

    
    countDict = {}

    for item in majorsli:
        if item in countDict:
            countDict[item] += 1
        else:
            countDict[item] = 1
    countList = countDict.values()
    lenCount = len(countList)
    maxCount = max(countList)

    modeList = []
    for item in countDict:
        if countDict[item] == maxCount:
            modeList.append(item)

    return modeList, lenCount

def majors_report(majors_mode,majors_ct,majors_li):
    '''(majors_mode: list, majors_ct: int, majors_li: list) -> None

    Reports the results of the data analysis.
    '''
    print(f'There are {majors_ct} in CIS this term')
    print(f'The most represented major is {majors_mode[0]}')
    print(f'{frequencyTable(majors_li)}')

    return
          
def frequencyTable(freqD):
    ''' (aList: list) -> None

    This function prints a frequency table
    '''
    
    print(freqD)
    #print(f"{'ITEM':<6} {'FREQUENCY':<9}")

    #for item in freqD:
    #    print(f'{item:<6} {freqD[item]:<9}')
        
    return

def main():
    '''() => None

    Calls: majors_readf, majors_analysis, majors_report

    Top level function for analysis of CIS 210 majors data.
    '''

    fname = 'p7a-majors-short.txt'
    #fname = 'p7a-majors.txt'

    majorsli = majors_readf(fname)                      #read
    majors_mode, majors_ct = majors_analysis(majorsli)  #analyze
    majors_report(majors_mode, majors_ct, majorsli)     #report
    return 
