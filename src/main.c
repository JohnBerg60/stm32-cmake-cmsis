#include <stdint.h>
#include "stm32f1xx.h"

uint32_t SystemCoreClock = 8000000;

// delay loop for 8 MHz CPU clock with optimizer enabled
void delay(uint32_t msec)
{
    for (uint32_t j = 0; j < 2000UL * msec; j++)
    {
        __NOP();
    }
}

int main(void)
{
    // Enable Port C and alternate functions
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

    // PC13 = Output for LEDs
    MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13 + GPIO_CRH_MODE13, GPIO_CRH_MODE13_0);

    while (1)
    {
        // LED Pin -> High
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);
        delay(500);

        // LED Pin -> Low
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR13);
        delay(500);
    }
}

void SystemInit()
{
    // Because the debugger switches PLL on, we may
    // need to switch back to the HSI oscillator without PLL

    // Switch to HSI oscillator
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);

    // Wait until the switch is done
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI)
    {
    }

    // Disable the PLL, then we can configure it
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

    // Flash latency 2 wait states
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_1);

    // Enable HSE oscillator
    SET_BIT(RCC->CR, RCC_CR_HSEON);

    // Wait until HSE oscillator is ready
    while (!READ_BIT(RCC->CR, RCC_CR_HSERDY))
    {
    }

    // 72 MHz using the 8 MHz HSE oscillator with 9x PLL, lowspeed I/O runs at 36 MHz
    WRITE_REG(RCC->CFGR, RCC_CFGR_PLLSRC + RCC_CFGR_PLLMULL9 + RCC_CFGR_PPRE1_DIV2);

    // Enable PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);

    // Wait until PLL is ready
    while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY))
    {
    }

    // Select PLL as clock source
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    // Update variable
    SystemCoreClock = 72000000;
}