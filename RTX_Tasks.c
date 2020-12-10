#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <stdio.h>
#include "stm32f10x_it.h"
#include "RTX_Tasks.h"
#include "controller.h"
#include "simulator.h"
#include "simulation.h"

/*----------------------------------------------------------------------------
  Task varaible: useful to keep track of tasks into the logic analyzer
 *----------------------------------------------------------------------------*/
volatile int BACKGROUND;
volatile int EMERGENCY_BRAKE;
volatile int STOP_SIGNAL;
volatile int STOP_SIGNAL_FINAL;
volatile int IDLE;
volatile int TRAFFIC_MESSAGE;
volatile int SIMULATION_EVENT;

extern int cmd;
extern int is_Sim_Active; 

/*----------------------------------------------------------------------------
  Declared Tasks
 *----------------------------------------------------------------------------*/
__task void BackgroundTask(void);
__task void EmergencyTask(void); 
__task void StopSignalTask(void);
__task void StopSignalFinalTask(void);
__task void TrafficMessagesTask(void);
__task void TaskInit(void);
__task void SimulationTask(void);

/*----------------------------------------------------------------------------
  Declared Tasks'ID
 *----------------------------------------------------------------------------*/
OS_TID background_Task_ID, emergency_Task_ID, stop_Signal_Task_ID, stop_Signal_Final_Task_ID, traffic_Messages_Task_ID, simulation_Task_ID;

/*----------------------------------------------------------------------------
  Initialize all tasks with a given priority
 *----------------------------------------------------------------------------*/
__task void TaskInit(void) {
	background_Task_ID = os_tsk_create(BackgroundTask, 10);
	emergency_Task_ID = os_tsk_create(EmergencyTask, 50); 
	stop_Signal_Task_ID = os_tsk_create(StopSignalTask, 20); 
	stop_Signal_Final_Task_ID = os_tsk_create(StopSignalFinalTask, 30);
	traffic_Messages_Task_ID = os_tsk_create(TrafficMessagesTask, 5);
	if (is_Sim_Active) {
		simulation_Task_ID = os_tsk_create(SimulationTask, 80);
	}
	
	os_tsk_delete_self(); // Task initializer kills self
}

/*----------------------------------------------------------------------------
  Background task: control the input from the lever 
 *----------------------------------------------------------------------------*/
__task void BackgroundTask() {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=1; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0; SIMULATION_EVENT=0;
		check_Lever_Input(); 
	}
}

/*----------------------------------------------------------------------------
  Emergency Brake task: handle the emergency brake request
 *----------------------------------------------------------------------------*/
__task void EmergencyTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 1; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0; SIMULATION_EVENT=0;
		handle_Emergency_Brake();		
	}
}

/*----------------------------------------------------------------------------
  Stop Signal task: handle the stop signal request
 *----------------------------------------------------------------------------*/
__task void StopSignalTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=1; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0; SIMULATION_EVENT=0;
		handle_Stop_Signal();
	}
}

/*----------------------------------------------------------------------------
  Stop Signal Final task: handle the stop signal request when 
													there's no input for more than 3 seconds.
 *----------------------------------------------------------------------------*/
__task void StopSignalFinalTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=1; IDLE=0; TRAFFIC_MESSAGE=0; SIMULATION_EVENT=0;
		handle_Stop_Signal_Final();
	}
}

/*----------------------------------------------------------------------------
  Traffic Messages task: handle the traffic messages over the serial line
 *----------------------------------------------------------------------------*/
__task void TrafficMessagesTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=1; SIMULATION_EVENT=0;
		printf("%c", cmd);
	}
}

/*----------------------------------------------------------------------------
  Initialize task: create and execute all the previous tasks
 *----------------------------------------------------------------------------*/
void initialize_Task(void) {
	os_sys_init(TaskInit);
}

/*----------------------------------------------------------------------------
  Simulation task: simulate and test the input into the system 
	
	- In order to run a different simulation change the current_Simulation	
    variable with one of the Simulation.h declaration 
 *----------------------------------------------------------------------------*/
__task void SimulationTask(void) {
	Simulator* current_Simulation = simulation_Stop_Signal; 
	Simulator sim;
	int size = 10;
	int i = 0;
	
	while (1) {
		
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0; SIMULATION_EVENT=1;
		sim = current_Simulation[i];
		i = (i+1)%size;
		create_Event(sim.event);
			
		if (sim.task_ID == 2) {
			os_evt_set(0x01, emergency_Task_ID);
		} else if (sim.task_ID == 3) {
			os_evt_set(0x01, stop_Signal_Task_ID);
		} else if (sim.task_ID == 4) {
			cmd = sim.event;
			os_evt_set(0x01, traffic_Messages_Task_ID);
		}
			
		os_dly_wait(sim.delay);
	}
}



