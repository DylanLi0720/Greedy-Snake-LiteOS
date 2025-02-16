#include "snake.h"

void game_init(Point** snake, uint8_t snake_length, Point* food) {
    oled_init();                 // 初始化 OLED
    oled_clear();                // 清空显示屏
    // 初始化蛇的初始位置
    (*snake)[0].x = 64;
    (*snake)[0].y = 32;
    (*snake)[1].x = 63;
    (*snake)[1].y = 32;
    (*snake)[2].x = 62;
    (*snake)[2].y = 32;

    generate_food(*snake, snake_length, food);             // 生成食物
    draw_snake(*snake, snake_length);                // 绘制初始蛇
    draw_food(*food);                 // 绘制食物
    // oled_drawRectangle(2,2,125,61,1);//画最外部边框
    oled_refresh_gram();
}

void generate_food(const Point* snake, uint8_t snake_length, Point* food) {
    uint8_t x, y;
    do {
        x = rand() % (OLED_WIDTH / 8);    // 随机生成 x 坐标
        y = rand() % (OLED_HEIGHT / 8);   // 随机生成 y 坐标
    } while (is_food_on_snake(snake, snake_length, x, y));    // 确保食物不与蛇重合
    (*food).x = x * 8;
    (*food).y = y * 8;
}

int is_food_on_snake(Point* snake, uint8_t x, uint8_t y, uint8_t snake_length) {
    for (int i = 0; i < snake_length; i++) {
        if (snake[i].x == x * 8 && snake[i].y == y * 8) {
            return 1;  // 食物和蛇重合
        }
    }
    return 0;  // 不重合
}

void draw_snake(const Point* snake, const snake_length) {
    for (int i = 0; i < snake_length; i++) {
        oled_draw_bigpoint(snake[i].x, snake[i].y, 1);  // 绘制蛇的每一部分
    }
}

void draw_food(const Point food) {
    // oled_drawpoint(food.x, food.y, 1);  // 绘制食物
    oled_draw_bigpoint(food.x, food.y, 1);
}

void move_snake(Point** snake, uint8_t snake_length, uint8_t direction) {
    // 移动蛇的身体
    for (int i = snake_length - 1; i > 0; i--) {
        (*snake)[i] = (*snake)[i - 1];  // 蛇身部分跟随前一部分
    }

    // 移动蛇头
    switch (direction) {
        case 1:  // 向右
            (*snake)[0].x += 4;
            break;
        case 2:  // 向下
            (*snake)[0].y += 4;
            break;
        case 3:  // 向左
            (*snake)[0].x -= 4;
            break;
        case 4:  // 向上
            (*snake)[0].y -= 4;
            break;
        default:
            break;
    }
}

int check_collision(Point* snake, uint8_t snake_length) {
    // 撞墙
    if (snake[0].x >= OLED_WIDTH || snake[0].y >= OLED_HEIGHT || snake[0].x < 0 || snake[0].y < 0) {
        return 1;  // 撞墙
    }

    // 撞到自己
    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;  // 撞到自己
        }
    }
    return 0;  // 无碰撞
}

void update_game(Point** snake, uint8_t* snake_length, uint8_t direction, Point* food) {
    move_snake(snake, *snake_length, direction);  // 移动蛇
    if (check_collision(*snake, *snake_length)) {
        oled_clear();  // 撞墙或撞到自己时清屏
        oled_showstring(10, 10, "GAME OVER", 16);  // 显示游戏结束
        oled_refresh_gram();
        return;
    }

    // 检查是否吃到食物
    if ((*snake)[0].x <= ((*food).x + 4) && (*snake)[0].x >= ((*food).x - 4) \
        && (*snake)[0].y <= ((*food).y + 4) && (*snake)[0].y >= ((*food).y - 4)) {
        (*snake_length)++;  // 蛇长一节
        generate_food(*snake, *snake_length, food);  // 重新生成食物
    }

    oled_clear();  // 清屏
    draw_snake(*snake, *snake_length);  // 绘制蛇
    draw_food(*food);   // 绘制食物
    // oled_drawRectangle(6,6,110,50,1);//画最外部边框
    oled_refresh_gram();
}

