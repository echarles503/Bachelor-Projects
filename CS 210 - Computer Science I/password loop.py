from time import sleep
'''
creating and validating a password
'''


'''
password = input("Enter a password: \n") 
sleep(.5)
print('...')
sleep(.5)
sentry = input("What is the password? \n")
while sentry != password:
    print("get tf outta here u bum!!!")
    sleep(1)
    sentry = input("What is the password? \n")
if sentry == password:
        print("you made it out the loop good looks\n \nand now ur logged tf in")
#enter = input(")
'''

correct = "stinkybaby"
tries = 0

keepgoing = True

while(keepgoing):

    tries += 1
    print("try #",tries)
    
    guess = input("what's the password?\n")

    if guess == correct:
        print("ur logged tf in, chief >:)")
        keepgoing = False
        
    elif tries >= 3:
        print("too many tries ur outta here :'(")
        keepgoing = False
