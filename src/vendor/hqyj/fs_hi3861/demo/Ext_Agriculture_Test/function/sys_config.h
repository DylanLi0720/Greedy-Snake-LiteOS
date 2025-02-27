/*
 * Copyright (c) 2023 Beijing HuaQing YuanJian Education Technology Co., Ltd
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H
#include <stdint.h>
#include "hi_adc.h"

#include "hal_bsp_structAll.h"

#define MOTOR_LOW_SPEED 40    // 低等速度
#define MOTOR_MIDDLE_SPEED 70 // 中等速度
#define MOTOR_HIGH_SPEED 100  // 高等速度

#define UDP_PORT 7788

// 小车的当前状态值

typedef enum {
    FLAMMABLE_STATUS_ON = 0x01,
    FLAMMABLE_STATUS_OFF,       
} te_combustible_status_t;


typedef enum {
    FLAME_STATUS_ON = 0x01, 
    FLAME_STATUS_OFF,       
} te_flame_status_t;

/*********************************** 系统的全局变量 ***********************************/


typedef struct _system1_value {
    te_flame_status_t flame_status; 
    te_combustible_status_t  combustible_status;  
    uint16_t CO2;
    uint16_t TVOC;
    uint16_t GAS_MIC;
} system_value_t;

extern system_value_t systemdata; // 系统全局变量


extern tn_pcf8574_io_t pcf8574_io;
#define IO_FAN pcf8574_io.bit.p0
#define IO_BUZZER pcf8574_io.bit.p1
#define IO_LED pcf8574_io.bit.p2

#endif
