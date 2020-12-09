#include "stm32f10x_it.h"
#include "RTX_Tasks.h"
#include "simulator.h"
#include "controller.h"
#include "lever.h"

int current_Event_Pin;

extern OS_TID background_Task_ID, emergency_Task_ID, stop_Signal_Task_ID, traffic_Messages_Task_ID, simulation_Task_ID;

Simulator simulation_Normal[10] = { {no_Acceleration_No_Braking, 1, 200},
																		{min_Acceleration, 1, 200},
																		{med_Acceleration, 1, 200},
																		{max_Acceleration, 1, 200},
																		{med_Acceleration, 1, 200},
																		{min_Acceleration, 1, 200},
																		{no_Acceleration_No_Braking, 1, 200},
																		{min_Braking, 1, 200},
																		{med_Braking, 1, 200},
																		{str_Braking, 1, 200}
																	};
																	
Simulator simulation_Stop_Signal[9] = { {no_Acceleration_No_Braking, 1, 100},
																				 {min_Acceleration, 1, 100},																				 
																				 {stop_Signal, 3, 100},
																				 {med_Acceleration, 1, 100},
																				 {0, 3, 100},
																				 {med_Acceleration, 1, 100},
																				 {no_Acceleration_No_Braking, 1, 100},
																				 {min_Acceleration, 1, 100},
																				 {med_Acceleration, 1, 100}
																			 };
																			 
Simulator simulation_Emergency_Brake[7] = { {no_Acceleration_No_Braking, 1, 100},
																						{min_Acceleration, 1, 100},
																						{med_Acceleration, 1, 100},
																						{max_Acceleration, 1, 100},
																						{emergency_Brake, 2, 100},
																						{med_Braking, 1, 100},
																						{str_Braking, 1, 100}
																					};

Simulator simulation_Max_Timer_Acceleration[6] = { {no_Acceleration_No_Braking, 1, 100},
																									 {min_Acceleration, 1, 100},
																									 {med_Acceleration, 1, 100},
																									 {max_Acceleration, 1, 500},
																									 {med_Acceleration, 1, 100},
																									 {min_Acceleration, 1, 100},																									 
																								 };

Simulator simulation_Max_Timer_No_Input[5] = { {no_Acceleration_No_Braking, 1, 100},
																							 {min_Acceleration, 1, 100},
																							 {0, 1, 500},
																							 {min_Braking, 1, 100},
																							 {med_Braking, 1, 100}																							
																						 };
																						 
Simulator simulation_Traffic_Messages[8] = { {no_Acceleration_No_Braking, 1, 100},
																						 {min_Braking, 1, 100},
																						 {'H', 4, 10},
																						 {'e', 4, 10},
																						 {'l', 4, 10},
																						 {'l', 4, 10},
																						 {'o', 4, 10},
																						 {med_Braking, 1, 100}																							
																					 };
																								 
void create_Event(int sim) {
	current_Event_Pin = (sim != 0) ? 1<<sim : 0; 
}

