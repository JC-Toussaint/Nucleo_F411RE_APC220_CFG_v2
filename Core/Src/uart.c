#include "uart.h"

char uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartRxCounter = 0;
uint8_t uartRxReadIdx = 0;
uint8_t uartRxWriteIdx = 0;
uint8_t uartRxOverflow = 0;
bool uartRxCr = false;

char uartTxBuffer[UART_TX_BUFFER_SIZE];
uint8_t uartTxCounter = 0;
uint8_t uartTxReadIdx = 0;
uint8_t uartTxWriteIdx = 0;

// call in void USARTX_IRQHandler(void)
void uartInterruptProcess()
{
    // receive
    if (LL_USART_IsActiveFlag_RXNE(USARTX) && LL_USART_IsEnabledIT_RXNE(USARTX))
    {
        uartRxBuffer[uartRxWriteIdx] = LL_USART_ReceiveData8(USARTX);

#if defined(UART_ECHO)
        uartPutChar(uartRxBuffer[uartRxWriteIdx]);
#endif

        if (uartRxBuffer[uartRxWriteIdx] == '\n')
        {
            uartRxCr = true;
        }

        uartRxCounter++;
        if (uartRxCounter == UART_RX_BUFFER_SIZE)
        {
            uartRxOverflow = 1;
            uartRxCounter = 0;
        }

        uartRxWriteIdx++;
        if (uartRxWriteIdx == UART_RX_BUFFER_SIZE)
            uartRxWriteIdx = 0;
    }

    // overrun error
    if (LL_USART_IsActiveFlag_ORE(USARTX))
    {
        LL_USART_ReceiveData8(USARTX);
        LL_USART_ClearFlag_ORE(USARTX);
    }

    // send
    if (LL_USART_IsEnabledIT_TXE(USARTX))
    {
        if (uartTxCounter)
        {
            LL_USART_TransmitData8(USARTX, uartTxBuffer[uartTxReadIdx]);
            uartTxCounter--;

            uartTxReadIdx++;
            if (uartTxReadIdx == UART_TX_BUFFER_SIZE)
                uartTxReadIdx = 0;
        }
        else
        {
            LL_USART_DisableIT_TXE(USARTX);
        }
    }
}

void uartTxFlush(void)
{
    uartTxCounter = 0;
    uartTxReadIdx = 0;
    uartTxWriteIdx = 0;
}

bool isUartRxNotEmpty(void)
{
    return (bool)uartRxCounter;
}

char uartGetChar(void)
{
    while (uartRxCounter == 0)
        ;

    char c = uartRxBuffer[uartRxReadIdx];
    uartRxCounter--;

    uartRxReadIdx++;
    if (uartRxReadIdx == UART_RX_BUFFER_SIZE)
        uartRxReadIdx = 0;

    return c;
}

void uartPutChar(char c)
{
    while (uartTxCounter == UART_TX_BUFFER_SIZE)
        ;

    LL_USART_DisableIT_TXE(USARTX);
    if (uartTxCounter || !LL_USART_IsActiveFlag_TXE(USARTX))
    {
        uartTxBuffer[uartTxWriteIdx] = c;
        uartTxWriteIdx++;
        uartTxCounter++;
        if (uartTxWriteIdx == UART_TX_BUFFER_SIZE)
            uartTxWriteIdx = 0;
        LL_USART_EnableIT_TXE(USARTX);
    }
    else
    {
        LL_USART_TransmitData8(USARTX, c);
    }
}

void uartSendStr(char *str)
{
    while (*str != '\0')
    {
        uartPutChar(*str);
        str++;
    }
}
