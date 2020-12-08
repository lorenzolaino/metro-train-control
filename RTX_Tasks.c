#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <stdio.h>
#include "stm32f10x_it.h"
#include "RTX_Tasks.h"
#include "swt.h"
#include "controller.h"
#include "simulator.h"

volatile int BACKGROUND;
volatile int EMERGENCY_BRAKE;
volatile int STOP_SIGNAL;
volatile int STOP_SIGNAL_FINAL;
volatile int IDLE;
volatile int TRAFFIC_MESSAGE;
volatile unsigned int SWT_STATE = 0;

unsigned int in_Pin_Stop_Signal=1<<1, out_Pin_Stop_Signal=1<<10;	
extern int cmd; 

__task void BackgroundTask(void);
__task void EmergencyTask(void); 
__task void StopSignalTask(void);
__task void StopSignalFinalTask(void);
__task void TrafficMessagesTask(void);
__task void TaskInit(void);
__task void SimulationTask(void);

OS_TID background_Task_ID, emergency_Task_ID, stop_Signal_Task_ID, stop_Signal_Final_Task_ID, traffic_Messages_Task_ID, simulation_Task_ID;

__task void TaskInit(void) {
	// Creates tasks
	background_Task_ID = os_tsk_create(BackgroundTask, 10);
	emergency_Task_ID = os_tsk_create(EmergencyTask, 50); 
	stop_Signal_Task_ID = os_tsk_create(StopSignalTask, 20); 
	stop_Signal_Final_Task_ID = os_tsk_create(StopSignalFinalTask, 30);
	traffic_Messages_Task_ID = os_tsk_create(TrafficMessagesTask, 5);
	simulation_Task_ID = os_tsk_create(SimulationTask, 80);
	
	os_tsk_delete_self(); // Task initializer kills self
}

__task void BackgroundTask() {
  
	while(1) {
		
		os_evt_wait_or(0x01, 0xFFFF);
		
		BACKGROUND=1; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0;
		
		SWT_STATE = 1-SWT_STATE;
		check_Input();
		
	}
}

__task void EmergencyTask(void) {
	
	while(1) {
		
		os_evt_wait_or(0x01, 0xFFFF);
		
		BACKGROUND=0; EMERGENCY_BRAKE = 1; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0;
		
		turn_Off_Led(1<<get_Current_State());

		GPIOC->ODR |= 1<<11;		// switch LED ON
	}
}

__task void StopSignalTask(void) {
	
	while(1) {
		
		os_evt_wait_or(0x01, 0xFFFF);
		
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=1; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=0;
		
		turn_Off_Led(1<<get_Current_State());

		if(GPIOB->IDR & in_Pin_Stop_Signal) {
			GPIOC->ODR |= out_Pin_Stop_Signal;		// switch LED ON
			set_Stop_Signal(1);
		} else {
			GPIOC->ODR &= ~out_Pin_Stop_Signal;   // switch LED OFF
			
			while(GPIOB->IDR != 1<<5);
			set_Stop_Signal(0);
		}
	}
}

__task void StopSignalFinalTask(void) {
	
	while(1) {
		
		os_evt_wait_or(0x01, 0xFFFF);
		
		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=1; IDLE=0; TRAFFIC_MESSAGE=0;
		
		turn_Off_Led(1<<get_Current_State());

		GPIOC->ODR |= out_Pin_Stop_Signal;		// switch LED ON
		set_Stop_Signal(1);
	}
	
}

__task void TrafficMessagesTask(void) {

	while(1) {
		
		os_evt_wait_or(0x01, 0xFFFF);

		BACKGROUND=0; EMERGENCY_BRAKE = 0; STOP_SIGNAL=0; STOP_SIGNAL_FINAL=0; IDLE=0; TRAFFIC_MESSAGE=1;
		printf("%c", cmd);
		
	}
	
}


void initialize_Task(void) {
	os_sys_init(TaskInit);
}

__task void SimulationTask(void) {
	
	while (1) {
		
		create_Simulation_1();
		create_Event();
		os_dly_wait( 100 );
		
	}
	
}
