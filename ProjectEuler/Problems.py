#!/usr/bin/python3

from math import factorial
import time

t0 = time.time()


def Fibonacci():
    a1, b1 = 0, 1
    yield a1
    yield b1
    while True:
        a1, b1 = b1, a1 + b1
        yield b1


def SubFib(start, end):
    for cur in Fibonacci():
        if cur > end: return
        if cur >= start:
            yield cur


def is_prime(number):
    number = abs(int(number))
    if number < 2:
        return False
    if number == 2:
        return True
    if not number & 1:
        return False
    for x in range(3, int(number ** 0.5) + 1, 2):
        if number % x == 0:
            return False
    return True


def primes(number):
    for i in range(2, number):
        if is_prime(i):
            yield i


def next_prime(prime_numb):
    prime_numb += 1
    while not is_prime(prime_numb):
        prime_numb += 1
    return prime_numb


print("1:", sum([i for i in range(1, 1000) if (i % 3 == 0 or i % 5 == 0)]))
print("2:", sum([c for c in SubFib(0, 4000000)]))

n, i = 600851475143, 0
for i in primes(int(n ** 0.5) + 1):
    if n % i == 0:
        n //= i
    if n == 1:
        break
print("3:", i)

n = 100
print("6:", n ** 2 * (n + 1) ** 2 // 4 - n * (n + 1) * (2 * n + 1) // 6)

a = 2
for i in range(1, 10001):
    a = next_prime(a)
print("7:", a)

print("9:", [a * (1000 - c - a) * c for c in range(0, 500) for a in range(1, (1000 - c) // 2)
             if (a ** 2 + (1000 - c - a) ** 2 == c ** 2)][0])

print("13:", str(sum(map(int, open('13', 'r').readlines())))[0:10])

print("16:", sum([int(n) for n in str(2 ** 1000)]))

print("20:", sum([int(n) for n in str(factorial(100))]))

print("97:", str(28433 * (2 ** 7830457 % 10 ** 10) + 1)[-10:])

t1 = time.time()

print("\nTotal time: ", "{:2.5f}".format(t1 - t0), "\n")
