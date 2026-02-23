#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void SystemClock_config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void Error_Handler(void);

#define FLASH_APP_ADDR 0x08004000
#define BOOTLOADER_VERSION "1.0.0"

#ifdef __cplusplus
}
#endif

#endif // MAIN_H