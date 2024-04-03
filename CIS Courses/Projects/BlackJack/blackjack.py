import random
import os
from time import sleep

class Card(object):
    cards = {
        'Ace' : 11,
        'King' : 10,
        'Queen' : 10,
        'Jack' : 10,
        '10' : 10,
        '9' : 9,
        '8' : 8,
        '7' : 7,
        '6' : 6,  
        '5' : 5,
        '4' : 4,
        '3' : 3,
        '2' : 2,
    }


    def __init__(self, suit, rank) -> None:
        self.rank = rank
        self.suit = suit
        self.points = self.cards[rank]

    def setpoints(self, p):
        self.points = p

    def getpoints(self):
        return self.points

    def __repr__(self) -> str:
        return f"{self.rank} - {self.suit}"

class Deck():

    def __init__(self) -> None:
        suits = ['Clubs', 'Spades', 'Hearts', 'Diamonds']
        ranks = ['Ace', 'King', 'Queen', 'Jack', '10', '9', '8', '7', '6', '5', '4', '3', '2']
        self.deck = []
        self.discard = []
        self.n = 1
        for i in range(self.n):
            for j in range(4):
                for k in range(13):
                    self.deck.append(Card(suits[j], ranks[k]))
    
    def count(self):
        return len(self.deck)

    def get(self):
        card = self.deck.pop()
        self.discard.append(card)
        return card

    def display(self):
        for card in self.deck:
            print(card)

    def shuffle(self):
        n = len(self.deck)
        tmp = [None] * 52
        shuffle = random.sample(range(n), n)
        i = 0
        for idx in shuffle:
            tmp[i] = self.deck[idx]
            i += 1
        self.deck = tmp
#╔═════════════╗   
#║ 9           ║
#║             ║      
#║             ║
#║      ♠      ║
#║             ║
#║             ║
#║           9 ║ 
#╚═════════════╝

#╔═════════════╗
#║             ║
#║             ║      
#║ ░░░░░░░░░░░ ║
#║ ░░░░░░░░░░░ ║
#║             ║
#║             ║
#║ ░░░░░░░░░░░ ║ 
#╚═════════════╝

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')
    return

def eval(hand: list, total: int):
    if total <= 21:
        return total
    ace = None
    for card in hand:
        if card.rank == 'Ace' and card.points != 1:
            card.points = 1
            total -= 10
        if total > 21:
            continue
    return total

def query():
    valid = ['hit', 'h', 'stand', 's', 'double', 'd']
    action = ""
    while action not in valid:
        action = input("Hit/Stand/Double (h/s/d)\n")
    if action == 'stand' or action == 's':
        return 0
    if action == 'hit' or action == 'h':
        return 1
    if action == 'double' or action == 'd':
        return 2

def resp(ptotal, dtotal, bank, bet):
    if dtotal == 0: # Dealer busts
        print("You win!")
        bank += (bet * 2)
        return bank
    if dtotal > ptotal: # Lose
        print("Dealer wins")
    elif dtotal < ptotal:  # Win
        bank += (bet * 2)
        print("You win!")
    else: # Push
        bank += bet
        print("Push.")
    return bank

def paction(d: Deck, phand: list, action: int, total: int):
    double = False
    if action == 2:
        double = not double
    while action:
        flop = d.get()
        phand.append(flop)
        total += flop.points
        total = eval(phand, total)
        print("You hit: ", flop)
        print("Current total: ", total, "\n")
        action = ""
        if double and total < 21:
            return total
        if (total > 21):
            print("Busted!")
            return 0
        if (total == 21):
            return total
    else:
        print("Current total: ", total, "\n")
        return total

def daction(d: Deck, dhand: list, total: int):
    print("Dealer's action!")
    print("Dealer flops: ", dhand[1])
    print("Dealer total: ", total)
    sleep(1)
    while total < 17:
        flop = d.get()
        total += flop.points
        eval(dhand, total)
        print("Dealer flops: ", flop)
        print("Dealer total: ", total)
        sleep(1)
    if total > 21:
        print("Dealer flops: ", flop)
        print("Dealer total: ", total)
        print("Dealer busted!")
        return 0
    return total



def main():
    d = Deck()
    d.shuffle()
    bank = 100
    #Ace = Card('Diamonds', 'Ace')
    while(1):
        if bank <= 0:
            print("House Wins!")
            return
        if d.count() <= 26:
            d = Deck()
            d.shuffle()
        print("BANK: ", bank)
        while True:
            try: 
                bet = int(input("Place your bet:\n"))
                break
            except ValueError:
                continue
        bank -= bet
        clear()
        print("BANK: ", bank, "\n")
        phand = [d.get(), d.get()]
        dhand = [d.get(), d.get()]
        ptotal = phand[0].points + phand[1].points
        dtotal = dhand[0].points + dhand[1].points
        print("Dealer's hand:\n", dhand[0])
        print("Your hand:\n", phand[0], phand[1])
        print("Current Total: ", ptotal)
        if ptotal == 21:
            print("Black Jack!")
            bank += (bet * 3/2)
            continue
        action = query()
        if action == 2:
            bank -= bet
            bet *= 2
        ptotal = paction(d, phand, action, ptotal)
        if ptotal == 0:
            print("Dealer wins")
            continue
        dtotal = daction(d, dhand, dtotal)
        bank = resp(ptotal, dtotal, bank, bet)
       
    return

if __name__ == "__main__":
    main()