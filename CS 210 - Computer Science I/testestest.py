def charCt(s,c):
    ct = 0
    for ch in s:
        if ch == c:
            ct += 1
        print(ct)
        return ct

charCt('hello, world', 'o')
