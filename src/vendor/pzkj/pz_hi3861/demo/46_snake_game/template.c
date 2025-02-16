/**
 ****************************************************************************************************
 * @file        template.c
 * @author      做梦咸鱼
 * @version     V1.0
 * @date        2025-02-16
 * @brief       贪吃蛇简易版游戏
 ****************************************************************************************************
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

#include "bsp_led.h"
#include "bsp_adc.h"
#include "bsp_key.h"
#include "hi_adc.h"
#include "bsp_oled.h"
#include "snake.h"

//管脚定义
#define ADC0_PIN         HI_IO_NAME_GPIO_12
#define ADC6_PIN         HI_IO_NAME_GPIO_13

osThreadId_t game_task_ID; //任务ID
osThreadId_t get_direction_task_ID; //按键及PS摇杆任务
osMessageQueueId_t direction_queue;

static uint8_t direction = 1;           // 蛇的初始方向，1 = 右，2 = 下，3 = 左，4 = 上
//ADC初始化
void adc0_6_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(ADC0_PIN, HI_IO_PULL_UP);                   // 设置GPIO上拉
    hi_io_set_pull(ADC6_PIN, HI_IO_PULL_UP);                   // 设置GPIO上拉
}
//读取AD值
uint16_t get_adc0_value(void)
{
    uint16_t data;
    hi_adc_read(HI_ADC_CHANNEL_0,&data,HI_ADC_EQU_MODEL_8,HI_ADC_CUR_BAIS_DEFAULT,0xff);
    return data;
}
uint16_t get_adc6_value(void)
{
    uint16_t data;
    hi_adc_read(HI_ADC_CHANNEL_6,&data,HI_ADC_EQU_MODEL_8,HI_ADC_CUR_BAIS_DEFAULT,0xff);
    return data;
}

//数据范围映射
int ratio_map(int x,int in_min,int in_max,int out_min,int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//方向判断
uint8_t get_direction_task(void)
{
    int adcx=0;
    int adcy=0;
    int adcsw=0;
    uint8_t key1=0;
    uint8_t key2=0;

    led_init();//LED初始化
    key_init();//按键初始化
    adc5_init();
    adc0_6_init();

    while(1){
        hi_gpio_get_input_val(KEY1_PIN,&key1);
        hi_gpio_get_input_val(KEY2_PIN,&key2);

        adcx=get_adc0_value();//读取ADC值
        adcx = ratio_map(adcx, 0, 4095, 0, 255);
        
        adcy=get_adc5_value();//读取ADC值
        adcy = ratio_map(adcy, 0, 4095, 0, 255);

        adcsw=get_adc6_value();//读取ADC值
        adcsw = ratio_map(adcsw, 0, 4095, 0, 255);

        // printf("adcx=%d\r\n",adcx);
        // printf("adcy=%d\r\n",adcy);
        // printf("adcsw=%d\r\n",adcsw);

        //根据输出值，自行调节阈值大小比较
        if(adcx<=100) direction = 1;//right方向
        if(adcy<=100) direction = 4;//up 方向
        if(key1 == 0) direction = 3;//左
        if(key2 == 0) direction = 2;//下
        // else if(adcsw<=100)i=5;//Button按下
        // osMessageQueuePut(direction_queue, &direction, 0, osWaitForever);
        // printf("direction of get_direction_task: %d\r\n",direction);
        usleep(100*1000);
    }
}


void game_task(void)
{
    Point* snake = (Point*)malloc(SNAKE_MAX_LENGTH * sizeof(Point));   // 蛇的坐标数组
    Point food;                      // 食物的坐标
    uint8_t snake_length = 3;        // 蛇的初始长度
    // char *state[6]={"home","up","down","left","right","pressed"};
    game_init(&snake, snake_length, &food);

    while (1) 
    {
        // osMessageQueueGet(direction_queue, &direction, NULL, osWaitForever);//未找到LIFO消息队列
        // printf("direction of game_task: %d\n",direction);
        update_game(&snake, &snake_length, direction, &food);
        usleep(200*1000);
    }
}
//任务创建
void oled_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "game_task";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal; // 任务的优先级

    game_task_ID = osThreadNew((osThreadFunc_t)game_task, NULL, &taskOptions); // 创建任务1
    if (game_task_ID != NULL)
    {
        printf("ID = %d, Create game_task_ID is OK!\n", game_task_ID);
    }
}

//任务创建
void getDirection_task_creation(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "get_direction_task";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal1; // 任务的优先级

    get_direction_task_ID = osThreadNew((osThreadFunc_t)get_direction_task, NULL, &taskOptions); // 创建任务1
    if (get_direction_task_ID != NULL)
    {
        printf("ID = %d, Create get_direction_task_ID is OK!\n", get_direction_task_ID);
    }
}
/**
 * @description: 初始化并创建任务
 * @param {*}
 * @return {*}
 */
static void template_demo(void)
{
    direction_queue = osMessageQueueNew(10, sizeof(uint8_t), NULL);
    if (direction_queue != NULL) {
        printf("ID = %d, Create direction_queue is OK!\n", direction_queue);
    }
    oled_task_create();
    getDirection_task_creation();
}
SYS_RUN(template_demo);
