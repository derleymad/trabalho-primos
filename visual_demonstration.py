import math
import pygame
import sys

pygame.init()

size = width, height = 1280, 720
backgroundColor = 230, 230, 230
black = 0, 0, 0
blockColor = 180, 180, 180
red = 255, 0, 0
blue = 0, 0, 255
green = 0, 255, 0
yellow = 0, 255, 255

clock = pygame.time.Clock()

screen = pygame.display.set_mode(size)
font = pygame.font.Font('font/Roboto-Medium.ttf', 16)

info = {
  "n": 120,
  "marginItems": 5,
  "marginTop": 50,
  "marginLeft": 20,
  "numberColumns": 13,
  "blockColor": (0, 0, 0)
}

def getPrime(array, n):
  root = math.ceil(math.pow(n, 0.5))
  for i in range(2, root):
    if(array[i]):
      colorNotPrime(n, i, array)
      
      

def colorNotPrime(n, i, array):
  for j in range(n):
    square = i * i
    index = square + (i * j)
    if(index < n):
      array[index] = False
    else:
      break

def sieve():
  primes = []
  n = info['n']
  array = [True] * n
  getPrime(array, n)
  for i in range(2, n):
    if(array[i]):
      primes.append(i)
  return primes


def write_text(text, x, y, color):
  text_surface = font.render(text, False, color)
  screen.blit(text_surface, (x, y))


def create_grid():
  container_width = width * 7.2/10
  n = info["n"]
  columns = math.ceil(math.pow(n * 1.5, 0.5))
  space = info["marginItems"]
  margin_top = info["marginTop"]
  margin_left = info["marginLeft"]
  side = math.floor((container_width - space) / columns)

  i = j = 1
  for index in range(1, n + 1):
    if(index == 1):
      i += 1
      continue
    if(index % columns == 1):
      j += 1
      i = 1
    x = int((i-1) * (side + space)) + margin_left
    y = int((j-1) * (side + space)) + margin_top

    pygame.draw.rect(screen, blockColor, [x, y, side, side])
    text_x = int(x + 8)
    text_y = int(y + 8)
    number = str(index)
    write_text(number, text_x, text_y, black)
    i += 1


running = True
counter = 0
while running:
  screen.fill(backgroundColor)
  create_grid()
  pygame.display.flip()
  for event in pygame.event.get():
    if event.type == pygame.QUIT:
      pygame.quit()
      running = False
  clock.tick(60)


print(sieve())
