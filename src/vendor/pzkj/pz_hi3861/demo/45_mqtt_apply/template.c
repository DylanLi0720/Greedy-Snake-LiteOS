#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "bsp_led.h"
#include "bsp_wifi.h"
#include "bsp_mqtt.h"
#include "bsp_pwm.h"
#include "bsp_adc.h"

#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/api_shell.h"

//控制任务
osThreadId_t PWM_Task_ID; //任务ID
osThreadId_t MODULE_Task_ID; //任务ID

// 消息队列
osMessageQueueId_t pwm_queue;
osMessageQueueId_t adc_queue;

void PWM_Task(void)
{
    int brightness_level;
    int pwm_val;
    led_init();//LED初始化
    pwm_init();//PWM初始化
    
    while (1) 
    {
        osMessageQueueGet(pwm_queue, &brightness_level, NULL, osWaitForever);
        pwm_val = brightness_level % 101 * 10;//限制输入上限
        printf("pwm_val = %d\n",pwm_val);
        if(pwm_val){
            pwm_set_duty(pwm_val);
        }
		else{
            pwm_set_duty(1);
        }
        usleep(10*1000);
    }
}
//任务创建
void pwm_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "pwmTask";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal; // 任务的优先级

    PWM_Task_ID = osThreadNew((osThreadFunc_t)PWM_Task, NULL, &taskOptions); // 创建任务
    if (PWM_Task_ID != NULL)
    {
        printf("ID = %d, Task Create OK!\n", PWM_Task_ID);
    }
}

//光敏检测任务
void MODULE_Task(void)
{
    uint8_t i=0;
    uint16_t adc_value=0;

    adc5_init();

    while (1) 
    {
        i++;
        if(i%50==0)
        {
            adc_value=get_adc5_value();
            // printf("adc_value=%d\r\n",adc_value);
            osMessageQueuePut(adc_queue, &adc_value, 0, osWaitForever);
        }
        usleep(10*1000);
    }
}

//任务创建
void module_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "ModuleTask";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal; // 任务的优先级

    MODULE_Task_ID = osThreadNew((osThreadFunc_t)MODULE_Task, NULL, &taskOptions); // 创建任务1
    if (MODULE_Task_ID != NULL)
    {
        printf("ID = %d, Create MODULE_Task_ID is OK!\n", MODULE_Task_ID);
    }
}

#define WIFI_SSID "HUAWEI Mate 60 Pro"
#define WIFI_PAWD "403403403"

#define SERVER_IP_ADDR "44.232.241.40"    //broker.emqx.io
#define SERVER_IP_PORT 1883
#define MQTT_TOPIC_SUB "subTopic"
#define MQTT_TOPIC_PUB "pubTopic"
#define TASK_INIT_TIME 2 // s
#define MQTT_RECV_TASK_TIME (200 * 1000) // us


osThreadId_t mqtt_send_task_id;   // mqtt订阅数据任务
osThreadId_t mqtt_recv_task_id;   // mqtt发布数据任务
int8_t mqtt_sub_payload_callback(unsigned char *topic, unsigned char *payload)
{
    int brightness_level = atoi(payload);
    printf("[info] topic:[%s]    recv<== %s\r\n", topic, payload);
    osMessageQueuePut(pwm_queue, &brightness_level, 0, osWaitForever);
}
void mqtt_recv_task(void)
{
    while (1) 
    {
        MQTTClient_sub();
        usleep(MQTT_RECV_TASK_TIME);
    }
}

void mqtt_send_task(void)
{
    uint8_t res=0;
    uint16_t light_val;
    char* str = (char*) malloc(100 * sizeof(char));
    // 连接Wifi
    if (WiFi_connectHotspots(WIFI_SSID, WIFI_PAWD) != WIFI_SUCCESS) 
    {
        printf("[error] WiFi_connectHotspots\r\n");
    }

    // 连接MQTT服务器
    if (MQTTClient_connectServer(SERVER_IP_ADDR, SERVER_IP_PORT) != 0) 
    {
        printf("[error] MQTTClient_connectServer\r\n");
    } 
    else 
    {
        printf("[success] MQTTClient_connectServer\r\n");
    }
    sleep(TASK_INIT_TIME);

    // 初始化MQTT客户端
    if (MQTTClient_init("mqtt_client_123", "username", "password") != 0) 
    {
        printf("[error] MQTTClient_init\r\n");
    } 
    else 
    {
        printf("[success] MQTTClient_init\r\n");
    }
    sleep(TASK_INIT_TIME);

    // 订阅Topic
    if (MQTTClient_subscribe(MQTT_TOPIC_SUB) != 0) 
    {
        printf("[error] MQTTClient_subscribe\r\n");
    } 
    else 
    {
        printf("[success] MQTTClient_subscribe\r\n");
    }
    sleep(TASK_INIT_TIME); 

    osThreadAttr_t options;
    options.name = "mqtt_recv_task";
    options.attr_bits = 0;
    options.cb_mem = NULL;
    options.cb_size = 0;
    options.stack_mem = NULL;
    options.stack_size = 1024*5;
    options.priority = osPriorityNormal;

    mqtt_recv_task_id = osThreadNew((osThreadFunc_t)mqtt_recv_task, NULL, &options);
    if (mqtt_recv_task_id != NULL) 
    {
        printf("ID = %d, Create mqtt_recv_task_id is OK!\r\n", mqtt_recv_task_id);
    }

    while (1) 
    {
        osMessageQueueGet(adc_queue, &light_val, NULL, osWaitForever);
        // strcpy(str, "the light level is ");
        sprintf(str, "the light level is %d", light_val); 
        // strcat(str, light_val);
        MQTTClient_pub(MQTT_TOPIC_PUB, str, strlen(str));
        sleep(TASK_INIT_TIME);
    }
}

//任务创建
void wifi_mqtt_task_create(void)
{
    osThreadAttr_t taskOptions;
    p_MQTTClient_sub_callback = &mqtt_sub_payload_callback;
    taskOptions.name = "mqttTask";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024*5;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal; // 任务的优先级

    mqtt_send_task_id = osThreadNew((osThreadFunc_t)mqtt_send_task, NULL, &taskOptions); // 创建任务
    if (mqtt_send_task_id != NULL)
    {
        printf("ID = %d, mqtt_send_task_id Create OK!\n", mqtt_send_task_id);
    }
}

/**
 * @description: 初始化并创建任务
 * @param {*}
 * @return {*}
 */
static void template_demo(void)
{
    pwm_queue = osMessageQueueNew(5, sizeof(int), NULL);
    if (pwm_queue != NULL) {
        printf("ID = %d, Create pwm_queue is OK!\n", pwm_queue);
    }
    adc_queue = osMessageQueueNew(5, sizeof(uint16_t), NULL);
    if (adc_queue != NULL) {
        printf("ID = %d, Create adc_queue is OK!\n", adc_queue);
    }
    module_task_create();
    pwm_task_create();
    wifi_mqtt_task_create();//任务创建
}
SYS_RUN(template_demo);
