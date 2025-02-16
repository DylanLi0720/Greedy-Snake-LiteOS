/**
 ****************************************************************************************************
 * @file        template.c
 * @author      普中科技
 * @version     V1.0
 * @date        2024-06-05
 * @brief       LiteOS消息队列
 * @license     Copyright (c) 2024-2034, 深圳市普中科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:普中-Hi3861
 * 在线视频:https://space.bilibili.com/2146492485
 * 公司网址:www.prechin.cn
 * 购买地址:
 *
 ****************************************************************************************************
 * 实验现象：打开串口助手，任务1发送消息成功后，任务2接收消息；
 *
 ****************************************************************************************************
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

// 动作类型
#define SCISSORS 0
#define ROCK 1
#define PAPER 2

osThreadId_t judge_task_ID; //  裁判任务 ID
osThreadId_t playerA_task_ID; //  玩家A任务 ID
osThreadId_t playerB_task_ID; //  玩家B任务 ID
osThreadId_t start_game_task_ID; //  游戏启动任务 ID
#define TASK_STACK_SIZE (1024)
#define TASK_DELAY_TIME 1 // s
#define MESSAGE_TIMEOUT 100
#define MsgQueueObjectNumber 16       // 定义消息队列对象的个数
// 消息队列
// typedef struct {
//     int playerA_choice;
//     int playerB_choice;
// } game_message_t;

// 信号量
osSemaphoreId_t game_start_sem;
// 消息队列
osMessageQueueId_t game_msg_queueA;
osMessageQueueId_t game_msg_queueB;

// 裁判任务
void judge_task(void) {
    // printf("enter judge_task\n");
    int choiceA, choiceB;
    int result;
    while (1) {
        // osSemaphoreAcquire(game_start_sem, osWaitForever);
        // printf("judge_task get a semaphore\n");
        osMessageQueueGet(game_msg_queueA, &choiceA, NULL, osWaitForever);
        osMessageQueueGet(game_msg_queueB, &choiceB, NULL, osWaitForever);

        result = (choiceA - choiceB + 3) % 3;

        if (result == 0) {
            printf("Round Result: Draw\n");
        } else if (result == 1) {
            printf("Round Result: Player A Wins\n");
        } else {
            printf("Round Result: Player B Wins\n");
        }
        // osSemaphoreRelease(game_start_sem);
        // sleep(TASK_DELAY_TIME);
    }
}

// 玩家A任务
void playerA_task(void) {
    // printf("enter playerA_task\n");
    int choice;
    char *str = (char*) malloc(10 * sizeof(char));
    // game_message_t msg;
    while (1) {
        osSemaphoreAcquire(game_start_sem, osWaitForever);
        // printf("playerA_task get a semaphore\n");
        choice = rand() % 3; // 0: Scissors, 1: Rock, 2: Paper
        switch (choice)
        {
        case 0:
            strcpy(str, "SCISSORS");
            break;
        case 1:
            strcpy(str, "ROCK");
            break;
        case 2:
            strcpy(str, "PAPER");
            break;
        default:
            break;
        }
        printf("Player A chose: %s\n", str);

        osMessageQueuePut(game_msg_queueA, &choice, 0, osWaitForever);
        osSemaphoreRelease(game_start_sem);
        // printf("playerA_task release a semaphore\n");
        sleep(TASK_DELAY_TIME);
    }
}

// 玩家B任务
void playerB_task(void) {
    // printf("enter playerB_task\n");
    char *str = (char*) malloc(10 * sizeof(char));
    int choice;
    // game_message_t msg;
    while (1) {
        osSemaphoreAcquire(game_start_sem, osWaitForever);
        choice = rand() % 3; // 0: Scissors, 1: Rock, 2: Paper
        switch (choice)
        {
        case 0:
            strcpy(str, "SCISSORS");
            break;
        case 1:
            strcpy(str, "ROCK");
            break;
        case 2:
            strcpy(str, "PAPER");
            break;
        default:
            break;
        }
        printf("Player B chose: %s\n", str);

        osMessageQueuePut(game_msg_queueB, &choice, 0, osWaitForever);

        osSemaphoreRelease(game_start_sem);
        sleep(TASK_DELAY_TIME);
    }
}

// 游戏启动任务
void start_game_task(void) {
    while (1) {
        // printf("enter start_game_task\n");
        // osSemaphoreAcquire(game_start_sem, osWaitForever);
        // sleep(TASK_DELAY_TIME); 
        osSemaphoreRelease(game_start_sem);
        sleep(TASK_DELAY_TIME); 
        // printf("start_game_task release a semaphore\n");

        osSemaphoreAcquire(game_start_sem, osWaitForever);
        // printf("start_game_task get a semaphore\n");
    }
}


/**
 * @description: 初始化并创建任务
 * @param {*}
 * @return {*}
 */
static void template_demo(void)
{
    game_start_sem = osSemaphoreNew(1, 0, NULL);  // 初始值为0，等待裁判启动信号

    if (game_start_sem != NULL) {
        printf("ID = %d, Create game_start_sem is OK!\n", game_start_sem);
    }
    game_msg_queueA = osMessageQueueNew(5, sizeof(int), NULL);

    if (game_msg_queueA != NULL) {
        printf("ID = %d, Create game_msg_queue is OK!\n", game_msg_queueA);
    }

    game_msg_queueB = osMessageQueueNew(5, sizeof(int), NULL);

    if (game_msg_queueB != NULL) {
        printf("ID = %d, Create game_msg_queue is OK!\n", game_msg_queueA);
    }

    osThreadAttr_t taskOptions;
    taskOptions.name = "judge_task";              // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = TASK_STACK_SIZE; // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal; // 任务的优先级

    judge_task_ID = osThreadNew((osThreadFunc_t)judge_task, NULL, &taskOptions);      // 创建任务1
    if (judge_task_ID != NULL) {
        printf("ID = %d, Create judge_task_ID is OK!\n", judge_task_ID);
    }

    taskOptions.name = "playerA_task";              // 任务的名字
    taskOptions.priority = osPriorityNormal1; // 任务的优先级
    playerA_task_ID = osThreadNew((osThreadFunc_t)playerA_task, NULL, &taskOptions);      // 创建任务2
    if (playerA_task_ID != NULL) 
    {
        printf("ID = %d, Create playerA_task_ID is OK!\n", playerA_task_ID);
    }

    taskOptions.name = "playerB_task";              // 任务的名字
    taskOptions.priority = osPriorityNormal1; // 任务的优先级
    playerB_task_ID = osThreadNew((osThreadFunc_t)playerB_task, NULL, &taskOptions);      // 创建任务2
    if (playerB_task_ID != NULL) 
    {
        printf("ID = %d, Create playerB_task_ID is OK!\n", playerB_task_ID);
    }

    taskOptions.name = "start_game_task";              // 任务的名字
    taskOptions.priority = osPriorityNormal2; // 任务的优先级
    start_game_task_ID = osThreadNew((osThreadFunc_t)start_game_task, NULL, &taskOptions);      // 创建任务2
    if (start_game_task_ID != NULL) 
    {
        printf("ID = %d, Create start_game_task_ID is OK!\n", start_game_task_ID);
    }
}
SYS_RUN(template_demo);
