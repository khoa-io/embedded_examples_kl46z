
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_QUEUE_ITEM (4)

typedef struct queue_element_struct
{
    uint8_t queueData[80];
} queue_element_struct_t;

typedef struct queue_data_struct
{
    uint8_t putIndex;
    uint8_t getIndex;
    uint8_t size;
    queue_element_struct_t element[MAX_QUEUE_ITEM];
} queue_data_struct_t;

static queue_data_struct_t queueList;

void queue_init(void)
{
    queueList.putIndex = 0;
    queueList.getIndex = 0;
    queueList.size = 0;
}

bool queue_isFull(void)
{
    bool retVal = true;
    if (queueList.size == MAX_QUEUE_ITEM)
    {
        retVal = true;
    }

    return retVal;
}

bool queue_isEmpty(void)
{
    bool retVal = false;

    if (queueList.size == 0)
    {
        retVal = true;
    }

    return retVal;
}

bool queue_getUnuse(uint8_t **outBuff)
{
    bool retVal = true;
    uint8_t putIndex = 0;

    if (!queue_isFull())
    {
        putIndex = queueList.putIndex % MAX_QUEUE_ITEM;
        *outBuff = (uint8_t *)&queueList.element[putIndex].queueData[0];
    }
    else
    {
        retVal = false;
    }

    return retVal;
}

bool queue_put(void)
{
    bool retVal = true;
    if (!queue_isFull())
    {
        queueList.putIndex++;
        queueList.size++;
    }
}

/* UART */
uint8_t *putPtr = NULL;
uint8_t index = 0;
void UART_init()
{
    queue_getUnuse(&putPtr);
}

void UART_ISR()
{
    putPtr[index] = UART0->D;
    if ("\n" == putPtr[index])
    {
        queue_put();
        queue_getUnuse(&putPtr);
        index = 0;
    }
    else
    {
        index++;
    }
}
