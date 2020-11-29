#include <stdbool.h> 

void initial_Configuration(void);
void check_Input(void);
void turn_On_Led(int out_pin);
void turn_Off_Led(int out_pin);
void set_Stop_Signal(bool is_Active); 
void set_Emergency_Breaking_Signal(bool is_Active);
void set_Current_State(int state);
int get_Current_State(void);
