#ifndef SNAKE_H
#define SNAKE_H

#include "cmsis_os2.h"
#include "bsp_oled.h"

#define OLED_WIDTH  128   // OLED 屏幕宽度
#define OLED_HEIGHT 64   // OLED 屏幕高度
#define SNAKE_MAX_LENGTH 100  // 最大蛇长度

typedef struct {
    uint8_t x;
    uint8_t y;
} Point;

// extern Point snake[SNAKE_MAX_LENGTH];   // 蛇的坐标数组
// extern uint8_t snake_length;        // 蛇的初始长度
// extern Point food;                      // 食物的坐标
// extern uint8_t direction;           // 蛇的初始方向，1 = 右，2 = 下，3 = 左，4 = 上


void game_init(Point** snake, uint8_t snake_length, Point* food);
void generate_food(const Point* snake, uint8_t snake_length, Point* food);
int is_food_on_snake(Point* snake, uint8_t snake_length, uint8_t x, uint8_t y);
void draw_snake(const Point* snake, const snake_length);
void draw_food(const Point food);
void move_snake(Point** snake, uint8_t snake_length, uint8_t direction);
int check_collision(Point* snake, uint8_t snake_length);
void update_game(Point** snake, uint8_t* snake_length, uint8_t direction, Point* food);
#endif
