import math

import pygame
import sys

pygame.init()

size = width, height = 720, 600
backgroundColor = 230, 230, 230
black = 0, 0, 0
blockColor = 180, 180, 180

screen = pygame.display.set_mode(size)
font = pygame.font.Font('font/Roboto-Medium.ttf', 16)

info = {
    "n": 120,
    "marginItems": 3,
    "marginTop": 15,
    "marginLeft": 10,
    "numberColumns": 11,
    "blockColor": (0, 0, 0)
}


def write_text(text, x, y, color):
    text_surface = font.render(text, False, color)
    screen.blit(text_surface, (x, y))


def create_rect(info):
    container_width = width * 7.2/10
    columns = info["numberColumns"]
    n = info["n"]
    rows = math.ceil(n / columns)
    space = info["marginItems"]
    margin_top = info["marginTop"]
    margin_left = info["marginLeft"]
    side = int(container_width / columns)

    for i in range(columns):
        for j in range(rows):
            x = int(i * (side + space)) + margin_left
            y = int(j * (side + space)) + margin_top
            pygame.draw.rect(screen, blockColor, [x, y, side, side])
            text_x = int(x + 16)
            text_y = int(y + 16)
            number = str(i + j * columns)
            write_text(number, text_x, text_y, black)
    return


running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            running = False
            break
    screen.fill(backgroundColor)
    create_rect(info)
    pygame.display.flip()

sys.exit()
