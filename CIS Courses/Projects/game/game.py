from random import randint
import os

BREAK = "+---------------------------------------------------------+"

def replay():
    ask = ""
    while ask != 'y' and ask != 'n':
        ask = input("New game? (y/n)\n")
    if ask == 'y':
        os.system('cls' if os.name == 'nt' else 'clear')
        game()
    return

def msg(s, var=0):
    if not var:
        print(BREAK)
        print(s)
        print(BREAK)
    else:
        print(BREAK)
        print(s, var)
        print(BREAK)

def prompt():
    while True:
        try:
            guess = int(input("Enter a #: (1-100)\n"))
            if guess <= 0 or guess > 100:
                continue
            else:
                break
        except ValueError:
            continue
        
    return guess

def response(g, s, t):
    if g == s:
        msg("\t\tCongrats you guessed it!")
        return 0
    if t == 6 and g != s:
        return 0

    print("Your are...")
    if g <= s + 5 and g >= s - 5:
        print("\tHot! -> ±5")
    elif g <= s + 10 and g >= s - 10:
        print("\tGetting warmer -> ±10")
    if g < s:
        print("\tToo low!")
    elif g > s:
        print("\tToo high!")
    return 1

def game():
    tries, resp = 1, 1
    guess = 0
    secret = randint(1,100)
    print("\n\tGuess the secret number in 6 tries: (1-100)")
    while guess != secret and tries <= 6 and resp:
        if tries != 6:
            msg("\t\t\tGuess #", tries)
        else:
            msg("\t\t\tLast turn!")
        guess = prompt()
        resp = response(guess, secret, tries)
        tries += 1
    
    if guess != secret:
        msg("\tTough luck, the secret number was: ", secret)
    replay()
    return


def main():
    game()
    return


if __name__ == "__main__":
    os.system('cls' if os.name == 'nt' else 'clear')
    main()