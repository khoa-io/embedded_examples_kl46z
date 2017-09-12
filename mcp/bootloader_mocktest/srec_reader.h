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
    /* Parsed termination */
    e_parseStatus_done = 0U,
    /* Parsed data */
    e_parseStatus_inprogress = 1U,
    /* Parsed error */
    e_parseStatus_error = 2U,
    /* Parsed unknown */
    e_parseStatus_undefined = 3U,
    /* Parsed header */
    e_parseStatus_start = 4U,
} parse_status_t;

/* Parsed data */
struct parsed_dat
{
    uint32_t address;
    uint8_t data[MAX_DATA_SIZE];
    uint8_t dataLength;
};

typedef struct parsed_dat parsed_dat_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Parse SREC data and produce parsed_dat_t.
 * See more at https://en.wikipedia.org/wiki/SREC_(file_format) and
 * https://upload.wikimedia.org/wikipedia/commons/f/f1/Motorola_SREC_Chart.png
 *
 * @param pInput [in] srec line.
 * @param pOutput [out] Output of parsed data. Note that when you pass pOutput,
 * all byte of pOutput must be set to 0.
 *
 * @return Returns one of these code:
 * - e_parseStatus_undefined – when initializing
 * - e_parseStatus_inprogress – when parsing S0 or 1,2,3,4,5 or 6 line
 * - e_parseStatus_done – when parsing S7 or 8, 9 line
 * - e_parseStatus_error – When error
 */
parse_status_t parseData(uint8_t pInput[], parsed_dat_t *pOutput);

#endif /* _SREC_READER_H_ */
