/*!
 * Assigment 3 - Exercise 1 - Entry point
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Calculate the area of a rectangle based on its perimeter and diagonal.
 * Formula: S = p*p/8 - d*d/2
 *
 * @param p The perimeter of the rectangle.
 * @param d The diagonal of the rectangle.
 *
 * @return Returns the area of the rectangle.
 */
static float rect_cal_area(float p, float d);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static float rect_cal_area(float p, float d)
{
    return p * p / 8 - d * d / 2;
}

int main(int argc, char *argv[])
{
    /* Perimeter of the rectangle */
    float perimeter;
    /* Diagonal of the rectangle */
    float diagonal;
    /* Area of the rectangle */
    float area;

    char msg[] = "The value inputs are not valid, please re enter new inputs";

    scanf("%f %f", &perimeter, &diagonal);

    if (diagonal > perimeter)
    {
        printf(msg);
        return 0;
    }

    area = rect_cal_area(perimeter, diagonal);
    if (area > 0)
    {
        printf("S=%.2f", area);
    }
    else
    {
        printf(msg);
    }

    return 0;
}
