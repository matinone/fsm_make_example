/**
 * @file oxygen_cell.h
 * @author Matias Brignone
 * @date 8 Feb 2021
 * @brief Header file for the Oxygen Cell module.
 *
 * Solution to Exercise C.1 from "Test Candidatos Developer Sr - Preguntas.docx".
 */


#ifndef _OXYGEN_CELL_H
#define _OXYGEN_CELL_H

/* ==================== Dependencies ==================== */

#include <stdint.h>

/* ==================== Macros of public constants ==================== */

#define MAX_OXYGEN_CELL_INDEX           (2)
#define MAX_OXYGEN_CELL_CAL_ATTEMPT     (5)
#define MIN_OXYGEN_CELL_ERROR           (-2)


/* ==================== Public structs and enums ==================== */

//! Possible oxygen cell states
typedef enum    {
    OXYGEN_CELL_STATE_RESET,                //!< Initial reset state (not calibrated)
    OXYGEN_CELL_STATE_CALIBRATING,          //!< Cell alibration in process
    OXYGEN_CELL_STATE_CALIBRATION_FAILED,   //!< Cell calibration failed after MAX_OXYGEN_CELL_CAL_ATTEMPT attemps
    OXYGEN_CELL_STATE_READY,                //!< Cell ready after a successful calibration
    OXYGEN_CELL_STATE_EMPTY,                //!< Cell empty (measure = 0 while being ready)
    OXYGEN_CELL_STATE_UNCONNECTED,          //!< Cell not connected
}   oxygen_cell_state_t;


//! Error codes for the oxygen cell module
typedef enum    {
    OXYGEN_CELL_ERROR_INVALID_MEASURE = MIN_OXYGEN_CELL_ERROR,  //!< Invalid measure because the oxygen cell is not calibrate
    OXYGEN_CELL_ERROR_MAX_INDEX,                                //!< Attempt to access an oxygen cell with a too high index
    OXYGEN_CELL_NO_ERROR,                                       //!< No error
}   oxygen_cell_error_t;


//! Struct to represent an oxygen cell
typedef struct    {
    uint8_t             index;          //!< Index of the oxygen cell 
    uint8_t             cal_attempt;    //!< Current calibration attempt
    oxygen_cell_state_t state;          //!< Current state
}   oxygen_cell_t;


/* ==================== Prototypes of public functions ==================== */

/**
 * @brief Public function to get the measure of an oxygen cell.
 * If the cell is not calibrated, the function will return
 * OXYGEN_CELL_ERROR_INVALID_MEASURE, indicating that the
 * measurement is not valid.
 * 
 * @param[in]   index   Index of the oxygen cell to read.
 * @param[out]  measure Oxygen cell measure value.
 */
int32_t oxygen_cell_get_measure(uint8_t index);


/**
 * @brief Public function to get the state of an oxygen cell.
 * 
 * @param[in]   index   Index of the oxygen cell to read.
 * @param[out]  state   Oxygen cell current state.
 */
oxygen_cell_state_t oxygen_cell_get_state(uint8_t index);


/**
 * @brief Periodic task that handles the state of all the
 * available oxygen cells.
 * 
 */
void oxygen_cell_task();


#endif /* _OXYGEN_CELL_H */
