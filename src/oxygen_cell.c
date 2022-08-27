/**
 * @file oxygen_cell.c
 * @author Matias Brignone
 * @date 8 Feb 2021
 * @brief Implementation file for the Oxygen Cell module.
 *
 * Solution to Exercise C.1 from "Test Candidatos Developer Sr - Preguntas.docx".
 */


/* ==================== Dependencies ==================== */

#include "oxygen_cell.h"
#include "external_functions.h"

/* ==================== Macros of private constants ==================== */

#ifdef DEBUG_LOG
#include <stdio.h>
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT
#endif

#define TASK_DELAY_MS   (1000 * 2)

/* ==================== Declaration of private variables ==================== */

static oxygen_cell_t oxygen_cell_array[MAX_OXYGEN_CELL_INDEX];


/* ==================== Prototypes of private functions ==================== */

/**
 * @brief Initialize the fields of a given oxygen cell.
 * 
 * @param[in]   oxygen_cell_ptr     Pointer to the oxygen cell to initialize.
 * @param[in]   index               Index to assign to the oxygen cell.
 */
static void oxygen_cell_init(oxygen_cell_t* oxygen_cell_ptr, uint8_t index);

/**
 * @brief Update the state of a given oxygen cell.
 * 
 * @param[in]   oxygen_cell_ptr     Pointer to the oxygen cell to update.
 */
static void oxygen_cell_fsm_update(oxygen_cell_t* oxygen_cell_ptr);


/* ==================== Implementations of public functions ==================== */

int32_t oxygen_cell_get_measure(uint8_t index) {

    // errors have a values that cant be confused with a real measure
    // otherwise there would be no way to tell the difference
    // all the error messages have negative values, assuming that
    // the measures will always be >= 0

    if (index >= MAX_OXYGEN_CELL_INDEX)    {
        return OXYGEN_CELL_ERROR_MAX_INDEX;
    }

    oxygen_cell_t* oxygen_cell_ptr = &(oxygen_cell_array[index]);

    if (oxygen_cell_ptr->state == OXYGEN_CELL_STATE_READY)  {
        return oxygen_cell_get_raw_measure(oxygen_cell_ptr);
    }
    else if (oxygen_cell_ptr->state == OXYGEN_CELL_STATE_EMPTY)  {
        // no need to read the measure again
        return 0;
    }
    else    {
        return OXYGEN_CELL_ERROR_INVALID_MEASURE;
    }
}

oxygen_cell_state_t oxygen_cell_get_state(uint8_t index)   {

    if (index >= MAX_OXYGEN_CELL_INDEX)    {
        return OXYGEN_CELL_STATE_UNCONNECTED;
    }
    else    {
        return oxygen_cell_array[index].state;
    }
}

void oxygen_cell_task() {

    // initialize oxygen cells
    for (uint8_t cell_index = 0; cell_index < MAX_OXYGEN_CELL_INDEX; cell_index++)   {
        oxygen_cell_init(&(oxygen_cell_array[cell_index]), cell_index);
        DEBUG_PRINT("Oxygen cell %d init done\n", cell_index);
    }

    // loop forever through all the oxygen cells
    while (1)   {

        delay(TASK_DELAY_MS);

        for (uint8_t cell_index = 0; cell_index < MAX_OXYGEN_CELL_INDEX; cell_index++)   {
            DEBUG_PRINT("Cell %d: status = %d | measure = %d\n", cell_index, oxygen_cell_get_state(cell_index), oxygen_cell_get_measure(cell_index));

            oxygen_cell_fsm_update(&(oxygen_cell_array[cell_index]));
        }

    }
}


/* ==================== Implementations of private functions ==================== */

void oxygen_cell_init(oxygen_cell_t* oxygen_cell_ptr, uint8_t index)  {
    
    oxygen_cell_ptr->index          = index;
    oxygen_cell_ptr->state          = OXYGEN_CELL_STATE_RESET;  // initial state
    oxygen_cell_ptr->cal_attempt    = 0;
}


void oxygen_cell_fsm_update(oxygen_cell_t* oxygen_cell_ptr) {

    switch (oxygen_cell_ptr->state) {

        case OXYGEN_CELL_STATE_RESET:
            // start calibration if the cell is connected
            if (oxygen_cell_is_connected(oxygen_cell_ptr))  {
                oxygen_cell_start_cal(oxygen_cell_ptr);
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_CALIBRATING;
                oxygen_cell_ptr->cal_attempt = 0;   // restart calibration attemps
            }
            else    {
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_UNCONNECTED;
            }

            break;
        
        case OXYGEN_CELL_STATE_CALIBRATING:
            // check if the calibration was successful
            if (oxygen_cell_is_connected(oxygen_cell_ptr))  {
                calibration_result_t cal_result = oxygen_cell_get_cal_result(oxygen_cell_ptr);
                oxygen_cell_ptr->cal_attempt++;
                if (cal_result == CALIBRATION_RESULT_OK)    {
                    oxygen_cell_ptr->state = OXYGEN_CELL_STATE_READY;
                }
                else if (oxygen_cell_ptr->cal_attempt > MAX_OXYGEN_CELL_CAL_ATTEMPT)  {
                    oxygen_cell_ptr->state = OXYGEN_CELL_STATE_CALIBRATION_FAILED;
                }
                // otherwise stay in CALIBRATING state to attempt again in the next execution
            }
            else    {
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_UNCONNECTED;
            }

            break;

        case OXYGEN_CELL_STATE_READY:
            if (!oxygen_cell_is_connected(oxygen_cell_ptr))  {
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_UNCONNECTED;
            }
            else if (oxygen_cell_get_raw_measure(oxygen_cell_ptr) == 0)  {
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_EMPTY;
            }

            break;

        // assuming that the cell must be unconnected to refill it
        // and that the cell must be unconnected to fix the calibration problem
        // so it will be stuck in these states until it is unconnected and connected again
        case OXYGEN_CELL_STATE_EMPTY:
        case OXYGEN_CELL_STATE_CALIBRATION_FAILED:
            if (!oxygen_cell_is_connected(oxygen_cell_ptr))  {
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_UNCONNECTED;
            }

            break;
        
        case OXYGEN_CELL_STATE_UNCONNECTED:
            // if the cell is connected again, it must be recalibrated
            if (oxygen_cell_is_connected(oxygen_cell_ptr))  {
                oxygen_cell_ptr->state = OXYGEN_CELL_STATE_RESET;
            }

            break;
        
        default:
            // assumed that the cell is unconnected
            oxygen_cell_ptr->state = OXYGEN_CELL_STATE_UNCONNECTED;

            break;
    }

}
