'''
Title: Earthquake Watch
Author: Elias Charles
Credits: n/a

This program reports on the mean median and mode of Earthquake data near Eugene, OR.
'''
import doctest
import statistics
from p6a_data_analysis import mean
from p6a_data_analysis import median
from p6a_data_analysis import mode
from p6a_data_analysis import genFrequencyTable
from p6a_data_analysis import drawTable
from p6a_data_analysis import frequencyTable

def equake_readf(fname):
    '''(fname:file)->list

    Reads file and compiles data into list
    '''

    with open(fname,'r') as myf:
        magnitudes = []
        for line in myf:
            data = line.split(',')
            mag = data[4]
            magnitudes.append(mag)
        myf.close()   
        del magnitudes[0]
    return magnitudes

def equake_analysis(magnitudes):
    '''(magnitudes: list) -> tuple

    Uses data from magnitude list to calculate mean, median, and mode
    '''
    magnitudes = [float(i) for i in magnitudes]

    mmm = ()
    
    a = mean(magnitudes)
    b = median(magnitudes)
    c = mode(magnitudes)
    
    

    return (a,b,c)

def equake_report(magnitude,mmm,minimag):
    '''(magnitude:list, mmm:tuple, minimag)

    Reports the total number of equakes, mean median mode, and prints a table
    '''
    mag_count = len(magnitude)
    mean = mmm[0]
    median = mmm[1]
    mode = mmm[2]
    

    print('Earthquake Data Analysis\n25 Years Ago to Present\n250km Centered at Eugene, OR\n')
    print(f'There have been {mag_count} earthquakes with magnitude {minimag} or higher\nover the past 25 years\n')
    print('Mean magnitude is:', mean)
    print('Median magnitude is:', median)
    print('Mode(s) of magnitudes is:', mode,'\n')      
    frequencyTable(magnitude)     
    return

def main():
    '''() -> None

    Calls: equake_readf, equake_analysis, equake_report

    Top level function for earthquake data analysis
    Returns None.
    '''
    fname = 'p7b-equakes25f.csv'
    minmag = 2.5
    #fname = 'p7b-equakes50f.csv'
    #minmag = 5.0

    emags = equake_readf(fname)
    mmm = equake_analysis(emags)
    equake_report(emags, mmm, minmag)
    return

main()
