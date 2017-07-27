#ifndef _STUDENT_H_
#define _STUDENT_H_

/*******************************************************************************
* Definitions
 ******************************************************************************/

#define MAX_NAME_LEN 256

typedef struct student_t
{
    int32_t id;
    int8_t name[MAX_NAME_LEN];
} student_t;
#endif /* _STUDENT_H_ */
