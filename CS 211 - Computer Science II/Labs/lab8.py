import re


def valid_email(s:str) -> bool:
    regex = r'^\D[\w]+@[\w]+\.(com|edu)'
    pattern = re.compile(regex)
    return not (pattern.match(s) is None)


def valid_pass(s: str) -> bool:
    if not re.search(r"\W", s):

        return False
    if not re.search(r"\D", s):
        return False
    if len(s) <= 10:
        return False
    return True


ask_email = input("Provide an email address:")
valid_email(ask_email)
while valid_email(ask_email) is False:
    ask_email = input("Invalid email address, try again:")
    valid_email(ask_email)

ask_pass = input("Input your password:")
valid_pass(ask_pass)
while valid_pass(ask_pass) is False:
    ask_pass = input("Invalid password, try again:")
    valid_pass(ask_pass)
print("Sign up was successful!")
