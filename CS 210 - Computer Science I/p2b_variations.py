from math import sqrt

def add_digits(n):
    '''
    Summates the digits of a 3-digit integer.
    '''
    first_digit = n // 100
    second_digit = n // 10 % 10
    third_digit = n % 10

    sum = first_digit + second_digit + third_digit
    print("The sum of the digits in", n, "is", sum)
    return sum

def profit(i):
    '''
    An old-style movie theatre has a simple profit function. Each customer pays $5 per ticket. Each
    performance costs the theatre $20, plus .50 per attendee. Write a function, profit, which returns the
    net profit per show, given the number of attendees (customers) at the show.
    '''
    profit = i * 5
    loss = 1/2 * i + 20
    net_profit = profit - loss
    print("The net profit for", i,"customers at the theater is", net_profit)
    return net_profit


def gravity(v):
    '''
    A parcel is thrown downward at a speed of v meters per second from an airplane at altitude 11000 m.
    As it falls, its distance from the ground is given by the formula -4.9t2 - vt + 11000, where t is the time in
    seconds since it was dropped. Write a function, gravity, to print the time it will take to reach the
    ground, rounded to two decimal places.
    '''
    altitude = 11000

    p1 = sqrt(v**2 - 4*-4.9*11000) #splitting up the equation for time into 2 parts
    p2 = (15.9 - p1)/-9.8           #with the intention to clarify calculations
    t = round(p2, 2)
    print("Time to ground from", altitude,"meters for", v,"m/sec is", t, "seconds.")
    return

def main():
    add_digits(302)
    profit(35)
    gravity(15.9)
    return
main()
