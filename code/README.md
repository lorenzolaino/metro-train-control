# Metro train controller

#### How to run the system? 
There are no mandatory steps to make the system work, the only request is to place the project directory in the directory where the libraries are located (STM32F10x_StdPeriph_Driver, CMSIS).

#### Optional: Activate or Deactivate simulation
Into the main of the **Blinky.c** file it is possible to activate or deactive the simulation
- Change the parameter of the _Configure_Simulation_ method in order to activate the test and the simulation
- **0**: simulation are deactivted
- **1**: simulation are activated

#### Optional: Choose a different simulation
Into the _SimulationTask_ method of the ***RTX_Tasks.c*** it is possible to change the current simulation
Change the _current_Simulation_ variable with one of the following declaration: 
- simulation_Normal
- simulation_Stop_Signal
- simulation_Emergency_Brake    
- simulation_Max_Timer_Acceleration
- simulation_Max_Timer_No_Input
- simulation_Traffic_Messages
