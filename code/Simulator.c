#include "stm32f10x_it.h"
#include "RTX_Tasks.h"
#include "simulator.h"
#include "controller.h"
#include "lever.h"

/*----------------------------------------------------------------------------
	Current state of the simulated input on GPIOB
 *----------------------------------------------------------------------------*/
int current_Event_Pin;

/*----------------------------------------------------------------------------
  Create Event: set the current state of the simulated GPIOB input pin
 *----------------------------------------------------------------------------*/																					 
void create_Event(int sim) {
	current_Event_Pin = (sim != 0) ? 1<<sim : 0; 
}

