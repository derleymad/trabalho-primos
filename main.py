import pygame
from math import floor, ceil

width = 1280
height = 720
square_dimension = 50
initial_x_pos = 10
initial_y_pos = 10
num_of_num = 200
black = (0, 0, 0)
white = (255,255,255)

grid_position_limit = floor(width/1.5)
num_of_squares_line = floor(grid_position_limit/(square_dimension + initial_x_pos))
num_lines = ceil(num_of_num/num_of_squares_line)

pygame.init()
display = pygame.display.set_mode((width,height))

def draw_square(x_pos, y_pos, width, height):
    pygame.draw.rect(display, white, [x_pos, y_pos, width, height])

def draw_grid(initial_x_pos, initial_y_pos, width, height):
    x_pos = initial_x_pos
    y_pos = initial_y_pos
    for i in range(num_lines):
        for j in range(num_of_squares_line):
            draw_square(x_pos, y_pos, width, height)
            x_pos += square_dimension + initial_x_pos
        x_pos = initial_x_pos 
        y_pos = square_dimension + (initial_y_pos*2)


while True:
    display.fill(black)
    draw_grid(initial_x_pos, initial_y_pos, square_dimension, square_dimension)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()

    pygame.display.update()