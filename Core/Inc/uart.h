#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_dma.h"

#include "stm32f4xx_ll_exti.h"

#define USARTX USART1
#define UART_RX_BUFFER_SIZE (128)
#define UART_TX_BUFFER_SIZE (128)
// #define UART_ECHO

extern char uartRxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t uartRxCounter;
extern uint8_t uartRxReadIdx;
extern uint8_t uartRxWriteIdx;
extern uint8_t uartRxOverflow;
extern bool uartRxCr;

extern char uartTxBuffer[UART_TX_BUFFER_SIZE];
extern uint8_t uartTxCounter;
extern uint8_t uartTxReadIdx;
extern uint8_t uartTxWriteIdx;

void lpUartInit();
void uartTxFlush(void);
bool isUartRxNotEmpty(void);
char uartGetChar(void);
void uartPutChar(char c);
void uartSendStr(char *str);
void uartInterruptProcess();
