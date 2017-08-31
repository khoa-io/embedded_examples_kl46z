#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define UART_0 (0)
#define UART_1 (1)
#define UART_2 (2)

#define UART_DATA_POLARITY_TX_MASK (0x01U)
#define UART_DATA_POLARITY_RX_MASK (0x10U)

#define UART_PARITY_TYPE_EVEN (0)
#define UART_PARITY_TYPE_ODD (1)

#define UART_TYPE_TRANSMITTER_MASK (0x01U)
#define UART_TYPE_RECEIVER_MASK (0x10U)

/*******************************************************************************
 * Structures And Types
 ******************************************************************************/

struct uart_conf
{
    /* Number of data bits */
    uint8_t sz;

    /* Parity enable */
    bool parityEnable;

    /* Parity type */
    uint8_t parityType;

    /* MSB or LSB first */
    bool msbf;

    /* Data polarity: bit0 - transmit, bit1 - receive */
    uint8_t polarity;

    /* Baud rate */
    uint32_t baudRate;

    /* Enable transmitter or receiver or both */
    uint8_t type;

    void (*onReceive)(void);
};

typedef struct uart_conf uart_conf_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Enable UARTx
 *
 * @param uartx UARTx. One of UART_0, UART_1, UART_2.
 */
void UART_enable(uint8_t uartx);

/*!
 * @brief Configure the UART0 control registers for the desired data format.
 *
 * @param uartx UARTx. One of UART_0, UART_1, UART_2.
 * @param conf UART's configuration.
 */
void UART_config(uint8_t uartx, uart_conf_t *conf);

/*!
 * @brief Send a byte using UARTx.
 *
 * @param uartx UARTx. One of UART_0, UART_1, UART_2.
 * @param b     Byte to send.
 */
void UART_sendByte(uint8_t uartx, uint8_t b);

/*!
 * @brief Send a byte array using UARTx.
 *
 * @param uartx UARTx. One of UART_0, UART_1, UART_2.
 * @param arr   Byte array to send.
 * @param sz    Size of array `arr`.
 */
void UART_sendBytes(uint8_t uartx, uint8_t *arr, uint8_t sz);

/*!
 * @brief Read a byte from UARTx.
 *
 * @param uartx [in]  UARTx. One of UART_0, UART_1, UART_2.
 * @param b     [in,out] Store read byte.
 */
void UART_readByte(uint8_t uartx, uint8_t *b);

/*!
 * @brief Read a byte array from UARTx.
 *
 * @param uartx UARTx. One of UART_0, UART_1, UART_2.
 * @param buff  Byte array to store read data.
 * @param sz    Number of bytes to read.
 */
void UART_readBytes(uint8_t uartx, uint8_t *buff, uint8_t sz);

/*!
 * @brief Read a byte array from UARTx until buffer is full or meet new line
 * ('\n') character.
 *
 * @param uartx UARTx. One of UART_0, UART_1, UART_2.
 * @param buff  Byte array to store read data.
 * @param sz    Number of bytes to read.
 *
 * @return Return number of read bytes.
 */
uint8_t UART_readLine(uint8_t uartx, uint8_t *buff, uint8_t sz);

#endif /* _UART_H_ */
