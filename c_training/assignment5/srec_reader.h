#ifndef _SREC_READER_H_
#define _SREC_READER_H_

/*******************************************************************************
* Definitions
******************************************************************************/
/* Each line (in file) is a record. Each record has max size which is defined
below */
#define MAX_RECORD_SIZE 516

/* Max size of data of a (parsed) record is 250 bytes. */
#define MAX_DATA_SIZE 250

/* Max number of line */
#define MAX_RECORD_NUMBER 100

/* Return code by parsing function */
typedef enum {
    e_parseStatus_done = 0U,
    e_parseStatus_inprogress = 1U,
    e_parseStatus_error = 2U,
    e_parseStatus_undefined = 3U
} parse_status_t;

/* Parsed data */
typedef struct
{
    uint32_t address;
    uint8_t data[MAX_DATA_SIZE];
    uint8_t dataLength;
} parse_data_struct_t;

typedef enum
{
    e_recordType_header_16 = 0U,
    e_recordType_data_16 = 1U,
    e_recordType_data_24 = 2U,
    e_recordType_data_32 = 3U,
    e_recordType_reversed = 4U,
    e_recordType_count_16 = 5U,
    e_recordType_count_24 = 6U,
    e_recordType_term_32 = 7U,
    e_recordType_term_24 = 8U,
    e_recordType_term_16 = 9U,
} recordType_t;

/*******************************************************************************
* API
******************************************************************************/
/*!
 * @brief Parse SREC data and produce parse_data_struct_t.
 *
 * @param pInput [in] srec line.
 * @param pOutput [out] Output of parsed data.
 *
 * @return Returns one of these code:
 * - e_parseStatus_undefined – when initializing
 * - e_parseStatus_inprogress – when parsing S0 or 1,2,3,4,5 or 6 line
 * - e_parseStatus_done – when parsing S7 or 8, 9 line
 * - e_parseStatus_error – When error
 */
parse_status_t parseData(uint8_t pInput[], parse_data_struct_t *pOutput);

#endif /* _SREC_READER_H_ */
