/**
 * @file external_functions.h
 * @author Matias Brignone
 * @date 8 Feb 2021
 * @brief External functions.
 *
 * The functions declared here are supposed to be implemented
 * (and documented) somewhere else.
 * Dummy versions are implemented in order to test the application.
 */

#ifndef _EXTERNAL_FUNCTIONS_H
#define _EXTERNAL_FUNCTIONS_H

/* ==================== Dependencies ==================== */

#include <time.h>
#include <stdbool.h>
#include "oxygen_cell.h"


/* ==================== Public structs and enums ==================== */

typedef enum    {
    CALIBRATION_RESULT_OK,
    CALIBRATION_RESULT_FAILED,
}   calibration_result_t;


/* ==================== Public functions ==================== */

void oxygen_cell_start_cal(oxygen_cell_t* oxygen_cell_ptr)  {
    // start calibration
}


calibration_result_t oxygen_cell_get_cal_result(oxygen_cell_t* oxygen_cell_ptr)  {

    if(oxygen_cell_ptr->index == 1) {
        return CALIBRATION_RESULT_FAILED;
    }

    return CALIBRATION_RESULT_OK;
}


uint32_t oxygen_cell_get_raw_measure(oxygen_cell_t* oxygen_cell_ptr)  {
    return 123456;
}


bool oxygen_cell_is_connected(oxygen_cell_t* oxygen_cell_ptr)   {

    if(oxygen_cell_ptr->index == 1) {
        return false;
    }

    return true;
}


void delay(int milliseconds)
{
    long pause;
    clock_t now, then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}


#endif /* _EXTERNAL_FUNCTIONS_H */
