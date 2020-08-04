from random import randint

rand = 10

def lcg():
    a = 1140671485
    c = 128201163
    m = 2**24
    global rand
    rand = (a*rand + c) % m
    return rand

def make_number(l):
    before_dot = lcg() % l
    result = ""
    if before_dot == 0:
        result = "0."
    else:
        for _ in range(before_dot):
            result += str(lcg() % 10)
        result += "."

    for _ in range(16):
        result += str(lcg() % 10)

    return result

for _ in range(1000):
    for i in range(1, 20):
        print(make_number(i))
