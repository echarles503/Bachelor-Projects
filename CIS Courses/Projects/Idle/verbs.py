with open('verb.txt', 'r') as rf:
    lines = [line for line in rf]

with open('verb-s.txt', 'w') as sf:
    with open('verb-ed.txt', 'w') as edf:
        with open('verb-ing.txt', 'w') as ingf:
            for line in lines:
                if line[-3:-1] == 'ed':
                    if line[-4] != 'e': 
                        edf.write(line)
                elif line[-4:-1] == 'ing':
                    ingf.write(line)
                elif line[-2] == 's':
                    sf.write(line)