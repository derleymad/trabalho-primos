import math
import multiprocessing
import time

startTime = time.time()

def sieve(n, begin = 2):
  print('starting')
  primes = []
  array = [True] * n
  root = math.ceil(math.pow(n, 0.5))
  for i in range(2, root):
    if(array[i]):
      for j in range(n):
        square = i * i
        index = square + (i * j)
        if(index < n):
          array[index] = False
        else:
          break
  for i in range(begin, n):
    if(array[i]):
      primes.append(i)
  print('ending')
  print(time.time() - startTime)
  return primes

def showTime():
  startTime = time.time()
  sieve(20000)
  print(time.time() - startTime)

def showPTime():
  p1 = multiprocessing.Process(target= sieve, args= (10000000,))
  p2 = multiprocessing.Process(target= sieve, args= (20000000, 10000001, ))
  p1.start()
  p2.start()

showPTime()


