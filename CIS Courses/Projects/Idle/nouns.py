"""
with open('nouns.txt', 'r') as rf:
    lines = [line for line in rf]
      
with open('pnouns.txt', 'w') as pwf:
    with open('snouns.txt', 'w') as swf:
        for line in lines:
            if line[-2] == 's':
                if line[-3] in 'aeious':
                    swf.write(line)
                else:
                    pwf.write(line)
            else:
                swf.write(line)

""" 
with open('concrete-nouns.txt', 'r') as f:
    lines = [line for line in f]

with open('concrete-nouns.txt', 'w') as f:
    for line in lines:
        if line[-2] != 's':
            f.write(line)