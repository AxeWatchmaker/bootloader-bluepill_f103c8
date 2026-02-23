#include "main.h"
#include "bootloader.h"

UART_HandleTypeDef huart1;
GPIO_InitTypeDef GPIO_InitStruct;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    if(HAL_GPEO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
        HAL_UART_Transmit(&huart1, (uint8_t)"Bootloader: Ready to receive firmware...\r\n", 43, 1000);

        if(receive_firmware() == 0) {
            HAL_UART_Transmit(&huart1, (uint8_t)"Bootloader: Firmware updated!\r\n", 32, 1000);
        }
    }

    uint32_t app_stack = *(uint32_t*)FLASH_APP_ADDR;
    uint32_t app_reset = *(uint32_t*)(FLASH_APP_ADDR + 4);

    if(app_stack >= 0x20000000 && app_stack <= 0x20005000 &&
    app_reset >= 0x08004000 && app_reset <= 0x08010000) {
        jump_to_app(FLASH_APP_ADDR);
    } else {
        while (1)
        {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            HAL_Delay(500);
        }
        
    }
}

void Error_Handler(void)
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(100);
    }
}