import threading
import time
import math


# n = 1000 #FUNCIONA GENERICO MAS SEM THREADING

# numeros1 = []

# def func():
#     global n,numeros1

#     for i in range(2, n + 1):
#         numeros1.append(i)


# def func2():
#     global n,numeros1

#     for x in numeros1:
#         if x <= int(math.sqrt(n)):
#             for y in numeros1:
#                 if y % x == 0 and y != x: #nao é primo 
#                     numeros1.remove(y)
#         else:
#             break


# func()
# func2()

# print(numeros1)
# print(len(numeros1))








MAX = 100  #FUNCIONA ATE 100 CRIVO 
numeros = []

def criaLista():
    global numeros,MAX

    for i in range (1,MAX+1):
        numeros.append(i)

criaLista()

def removeMultiplos2():
    global numeros
    for i in numeros:
        if i % 2 == 0 and i > 2 :
            numeros.remove(i)
def removeMultiplos3():
    global numeros
    for i in numeros:
        if i % 3 == 0 and i > 3 :
            numeros.remove(i)
def removeMultiplos5():
    global numeros
    for i in numeros:
        if i % 5 == 0 and i > 5 :
            numeros.remove(i)
def removeMultiplos7():
    global numeros
    for i in numeros:
        if i % 7 == 0 and i > 7 :
            numeros.remove(i)


threading.Thread(target=removeMultiplos2).start() # threading da remocao de cada multiplo de maneira nao sequencial das 4 funcoes a baixo (paralela)
removeMultiplos3()
removeMultiplos5()
removeMultiplos7()

print(len(numeros))

print((numeros))





