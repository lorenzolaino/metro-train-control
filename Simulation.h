/*----------------------------------------------------------------------------
  Simulation normal: simulate the normal work of the train with a complete 
										 acceleration e a complete deceleration.
 *----------------------------------------------------------------------------*/
Simulator simulation_Normal[10] = { {no_Acceleration_No_Braking, 1, 100},
																		{min_Acceleration, 1, 100},
																		{med_Acceleration, 1, 100},
																		{max_Acceleration, 1, 100},
																		{med_Acceleration, 1, 100},
																		{min_Acceleration, 1, 100},
																		{no_Acceleration_No_Braking, 1, 100},
																		{min_Braking, 1, 100},
																		{med_Braking, 1, 100},
																		{str_Braking, 1, 100}
																	};

/*----------------------------------------------------------------------------
  Simulation Stop Signal: simulate the stop signal request during an 
													acceleration: brakes are activated. 
													Other requests are ignored until the clearance of the 
													flag and the lever in idle position.
 *----------------------------------------------------------------------------*/																	
Simulator simulation_Stop_Signal[10] = { {no_Acceleration_No_Braking, 1, 100},
																				 {min_Acceleration, 1, 100},																				 
																				 {stop_Signal, 3, 100},
																				 {med_Acceleration, 1, 100},
																				 {no_Input, 3, 100},		
																				 {min_Acceleration, 1, 100},
																				 {med_Acceleration, 1, 100},
																				 {no_Acceleration_No_Braking, 1, 100},
																				 {min_Acceleration, 1, 100},
																				 {med_Acceleration, 1, 100}
																			 };

/*----------------------------------------------------------------------------
  Simulation Emergency Brake: simulate the emergency brake request during an
															acceleration: maximum brakes are activated.
															Other requests are ignored, the system needs a 
															manual reset. 
 *----------------------------------------------------------------------------*/																			 
Simulator simulation_Emergency_Brake[10] = { {no_Acceleration_No_Braking, 1, 100},
																						 {min_Acceleration, 1, 100},
																						 {med_Acceleration, 1, 100},
																						 {max_Acceleration, 1, 100},
																						 {emergency_Brake, 2, 100},
																						 {med_Braking, 1, 100},
																						 {str_Braking, 1, 100},
																						 {med_Braking, 1, 100},
																						 {min_Braking, 1, 100},
																						 {no_Acceleration_No_Braking, 1, 100}
																					};

/*----------------------------------------------------------------------------
  Simulation Timer Acceleration: simulate the timeout of the maximum
																 acceleration force: after 4 consecutive 
																 seconds of maximum acceleration, even if lever
																 is on the same position, acceleration is 
																 decreased to medium.
 *----------------------------------------------------------------------------*/																			
Simulator simulation_Max_Timer_Acceleration[10] = { {no_Acceleration_No_Braking, 1, 100},
																									  {min_Acceleration, 1, 100},
																									  {med_Acceleration, 1, 100},
																									  {max_Acceleration, 1, 500},
																									  {med_Acceleration, 1, 100},
																									  {min_Acceleration, 1, 100},
																										{med_Acceleration, 1, 100},
																										{max_Acceleration, 1, 500},
																										{med_Acceleration, 1, 100},
																									  {min_Acceleration, 1, 100}
																								 };

/*----------------------------------------------------------------------------
  Simulation Timer No Input: simulate the timeout of the no lever input: 
														 after 3 consecutive seconds of no input train is 
														 stopped with a stop signal. Other requests are
														 ignored, the system needs a manual reset.  
 *----------------------------------------------------------------------------*/
Simulator simulation_Max_Timer_No_Input[10] = { {no_Acceleration_No_Braking, 1, 100},
																							  {min_Acceleration, 1, 100},
																								{med_Acceleration, 1, 100},
																								{min_Acceleration, 1, 100},
																							  {no_Input, 1, 400},
																							  {min_Braking, 1, 100},
																							  {med_Braking, 1, 100},
																								{min_Braking, 1, 100},
																								{no_Acceleration_No_Braking, 1, 100},
																								{min_Acceleration, 1, 100}
																						 };

/*----------------------------------------------------------------------------
  Simulation Traffic Messages: simulate the receiving of traffic messages 
															 from the serial line USART1. 
 *----------------------------------------------------------------------------*/																						 
Simulator simulation_Traffic_Messages[10] = { {no_Acceleration_No_Braking, 1, 100},
																						  {min_Braking, 1, 100},
																						  {'T', 4, 10},
																						  {'r', 4, 10},
																							{'a', 4, 10},
																							{'f', 4, 10},
																						  {'f', 4, 10},
																						  {'i', 4, 10},
																						  {'c', 4, 10},
																						  {str_Braking, 1, 100}																							
																					 };				
																					 
