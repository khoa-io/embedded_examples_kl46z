# Assignment 5

Write a program to parse s-record file. Print out the info:
- Line number
- Converted address
- Converted data

Requirement:
Not allow to use functions to allocate dynamic memory such as malloc, calloc.


The prototype of parse function is required as below:

```c
typedef enum {
 e_parseStatus_done = 0U,
 e_parseStatus_inprogress = 1U,
 e_parseStatus_error = 2U,
 e_parseStatus_undefined = 3U
} parse_status_t;

typedef struct
{
 uint32_t address;
 uint8_t data[MAX_DATA_SIZE];
 uint8_t dataLength;
} parse_data_struct_t;

parse_status_t parseData(uint8_t pInput[], parse_data_struct_t *pOutput);
```

Param:

- `pInput[]` – srec line

- `pOutput` – output of parsed data

Return:
- Undefined – when initializing
- Inprogress – when parsing S0 or 1,2,3,4,5 or 6 line
- Done – when parsing S7 or 8, 9 line
- Error – When error


Reference:
[SREC_(file_format)](https://en.wikipedia.org/wiki/SREC_(file_format))
