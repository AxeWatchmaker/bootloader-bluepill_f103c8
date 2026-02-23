#include "bootloader.h"
#include "flash_if.h"

typedef void (*pFunction)(void);

int recevie_firmware(void)
{
    uint8_t buffer[256];
    uint32_t app_size;
    uint32_t address = FLASH_APP_ADDR;

    HAL_FLASH_Unlock();

    HAL_UART_Receive(&huart1, (uint8_t*)&app_size, 4, HAL_MAX_DELAY);

    FLASH_EraseInitTypeDef erase;
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = FLASH_APP_ADDR;
    erase.NbPages = 12;
    uint32_t page_error;

    if(HAL_FLASHEx_Erase(&erase, &page_error) != HAL_OK) {
        HAL_FLASH_Lock();
        return -1;
    }

    for(uint32_t i = 0; i < app_size; i += 256) {
        HAL_UART_Receive(&huart1, buffer, 256, HAL_MAX_DELAY);

        for(int j = 0; j < 256; j += 4) {
            uint32_t word = buffer[j] | (buffer[j+1] << 8) |
                            (buffer[j+2] << 16) | (buffer[j+3] << 24);

            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i + j, word) != HAL_OK) {
                HAL_FLASH_Lock();
                return -2;
            }
        }
    }

    HAL_FLASH_Lock();
    return 0;
}

void jump_to_app(uint32_t app_address)
{
    __disable_irq();

    HAL_RCC_DeInit();
    HAL_DeInit();

    for(int i = 0; i < 8; i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
    }

    __DSB();
    __ISB();

    uint32_t app_stack = *(uint32_t*)app_address;

    pFunction app_reset_handler = (pFunction)*(uint32_t*)(app_address + 4);

    SCB->VTOR
}