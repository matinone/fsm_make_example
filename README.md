# fsm_make_example
An example C program using a simple FSM (Finite State Machine) with a small standalone makefile to use as reference / starting point.

The FSM represents an oxygen cell in a mechanical ventilator, which goes through different states according to the following rules:
* When the machine starts, the cell is not calibrated.
* The cell calibration process can be successful, or there might be some external or internal problem (cell malfunctioning).
* Cell values are only valid if the cell is calibrated.
* During normal functioning, the cell may run out or be unplugged by the user.
* If the cell is changed (or plugged back in), it must be recalibrated in order for the measurements to be valid again.

## Assumptions
* The function `oxygen_cell_task` is a "task" that runs periodically and handles the state of all the oxygen cells, iterating all of them each time it runs. In the example, it is the only running task and it has a `while(1)` inside, as if it was running as part of a RTOS (Real Time Operating System).
* The header `external_functions.h` declares functions that would be defined, implemented and documented in separate modules. Some simple implementations are made in order to be able to test the expected behavior.
* Regarding the oxygen cell itself:
   * It is assumed that the lectures would always be greater than or equal to zero, so if a negative value is received, it can be unequivocally interpreted as an error message.
   * If the cell calibration fails or if the cell runs out, it must be disconnected and reconnected to make it work again.

## How to compile/run it:
* Compile without debug messages: `make`
* Compile with debug messages: `make debug_log`
* Remove all the generated files: `make clean`
* Run it: `./main.out`

