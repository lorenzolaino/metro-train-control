#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <stdio.h>
#include "stm32f10x_it.h"
#include "RTX_Tasks.h"
#include "controller.h"
#include "simulator.h"

volatile int BACKGROUND;
volatile int EMERGENCY_BRAKE;
volatile int STOP_SIGNAL;
volatile int STOP_SIGNAL_FINAL;
volatile int IDLE;
volatile int TRAFFIC_MESSAGE;

unsigned int in_Pin_Stop_Signal=1<<1, out_Pin_Stop_Signal=1<<10;	
unsigned int pin_Lever_Idle=1<<5;

extern int cmd; 

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
	simulation_Task_ID = os_tsk_create(SimulationTask, 80);
	
	os_tsk_delete_self(); // Task initializer kills self
}

/*----------------------------------------------------------------------------
  Background task: control the input from the lever 
 *----------------------------------------------------------------------------*/
__task void BackgroundTask() {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=1; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0;
		check_Input();
	}
}

/*----------------------------------------------------------------------------
  Emergency Brake task: handle the emergency brake request
 *----------------------------------------------------------------------------*/
__task void EmergencyTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 1; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0;
		turn_Off_Led(1<<get_Current_State());
		turn_On_Led(1<<11);		
	}
}

/*----------------------------------------------------------------------------
  Stop Signal task: handle the stop signal request
 *----------------------------------------------------------------------------*/
__task void StopSignalTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=1; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0;
		turn_Off_Led(1<<get_Current_State());

		if(GPIOB->IDR & in_Pin_Stop_Signal) {
			turn_On_Led(out_Pin_Stop_Signal);		
			set_Stop_Signal(1);
		} else {
			turn_Off_Led(out_Pin_Stop_Signal);   
			
			while(GPIOB->IDR != pin_Lever_Idle);
			set_Stop_Signal(0);
		}
	}
}

/*----------------------------------------------------------------------------
  Stop Signal Final task: handle the stop signal request when 
													there's no input for more than 3 seconds.
 *----------------------------------------------------------------------------*/
__task void StopSignalFinalTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=1; IDLE=0; TRAFFIC_MESSAGE=0;
		turn_Off_Led(1<<get_Current_State());
		turn_On_Led(out_Pin_Stop_Signal);
		set_Stop_Signal(1);
	}
}

/*----------------------------------------------------------------------------
  Traffic Messages task: handle the traffic messages over the serial line
 *----------------------------------------------------------------------------*/
__task void TrafficMessagesTask(void) {
	while(1) {
		os_evt_wait_or(0x01, 0xFFFF);
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=1;
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
 *----------------------------------------------------------------------------*/
__task void SimulationTask(void) {
	while (1) {
		create_Simulation_1();
		create_Event();
		os_dly_wait( 100 );
	}
}
