import sys, os
from random import randint
from youvegotmail import send
BREAK = "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n"

def checkflag(s):
    flag = False
    while (True):
        ask = input(s).strip()
        if ask == 'y' or ask == 'yes':
            flag = True
            break
        elif ask == 'n' or ask == 'no':
            flag = False
            break
    return flag

def procPoem(map):
    with open('lovepoem.txt', 'r') as f:
        lines = [line.strip().split(" ") for line in f]
        N = len(lines)
    headers = []
    ctr = 0
    # Get the blanks order from first word of line
    for word in lines:
        headers.append(word[0])
        word.remove(word[0])
    
    # Split the blanks by '|'
    for x in range(len(headers)):
        headers[x] = headers[x].split("|")

    for line in range(N):
        for x in range(len(lines[line])):
            word = lines[line][x]
            if word == '$$':
                bank = headers[line][ctr]   # determine blank type
                n = len(map[bank])  # get length of word bank
                i = randint(0, n-1) # calculate random index
                lines[line][x] = map[bank][i]
                ctr += 1
        lines[line] = ' '.join(lines[line])
        lines[line] += "\n"
        ctr = 0

    return lines

def main(map):
    lines = procPoem(map)
    print(BREAK)
    for line in lines:
        print(line)
    print(BREAK)

    flag = checkflag("Do you like this poem?\n")
    if flag:    
        with open("out.txt", "w") as f:
            for line in lines:
                f.write(line)
        share = checkflag("Do you want to share this poem?\n")
        if share:
            send("out.txt")
            return
    else:
        rerun = checkflag("Generate another poem?\n")
        if rerun:
            main(map)
        else:
            return

if __name__ == "__main__":
    with open('concrete-nouns.txt', 'r') as f:
        cnouns = [line.strip().lower() for line in f.readlines()]
    with open('snouns.txt', 'r') as f:
        snouns = [line.strip() for line in f.readlines()]
    with open('pnouns.txt', 'r') as f:
        pnouns = [line.strip() for line in f.readlines()]
    with open('verb-ed.txt', 'r') as f:    
        verbed = [line.strip() for line in f.readlines()]
    with open('verb-s.txt', 'r') as f:    
        sverb = [line.strip() for line in f.readlines()]
    with open('verb-ing.txt', 'r') as f:    
        verbing = [line.strip() for line in f.readlines()]
    with open('adj.txt', 'r') as f:    
        adjs = [line.strip() for line in f.readlines()]
    wordmap = {}
    wordmap["CNOUN"] = cnouns
    wordmap["SNOUN"] = snouns
    wordmap["PNOUN"] = pnouns
    wordmap["VERB-S"] = sverb
    wordmap["VERB-ING"] = verbing
    wordmap["VERB-ED"] = verbed
    wordmap["ADJ"] = adjs

    main(wordmap)

