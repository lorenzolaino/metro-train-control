#include "stm32f10x_it.h"
#include "RTX_Tasks.h"
#include "simulator.h"
#include "controller.h"
#include "lever.h"

extern OS_TID background_Task_ID, emergency_Task_ID, stop_Signal_Task_ID, traffic_Messages_Task_ID, simulation_Task_ID;

typedef struct {
	int event;
	int task_ID;
	int delay;
} Simulator;

const Simulator idle_Event = {no_Acceleration_No_Braking, 1, 200};
const Simulator min_Acc_Event = {min_Acceleration, 1, 200};
const Simulator med_Acc_Event = {med_Acceleration, 1, 200};
const Simulator max_Acc_Event = {max_Acceleration, 1, 6000};
const Simulator min_Brak_Event = {min_Braking, 1, 200};
const Simulator med_Brak_Event = {med_Braking, 1, 200};
const Simulator str_Brak_Event = {str_Braking, 1, 200};
const Simulator stop_Signal_Event = {stop_Signal, 3, 200};
const Simulator emergency_Brake_Event = {emergency_Brake, 2, 200};

Simulator simulation_1[2];

void create_Simulation_1(void) {
	simulation_1[0] = idle_Event;
	simulation_1[1] = min_Acc_Event;
}

void create_Event(void) {
	
	//for (int i = 0; i < sizeof(simulator_Event); i++) {
	//GPIOB->ODR ^= 1<<8;
	GPIOB->BSRR = 1<<8;
	//}
	
}

