#include "swt.h"

typedef struct {
  unsigned long int period;
  unsigned long int counter;
  int               started;
  int               flag;
} swt_t;

static swt_t timers[MAX_TIMERS];

void swtInit( void ) {
  int i;
  for( i = 0; i < MAX_TIMERS; i++ ) {
    timers[i].period  = 0;
    timers[i].counter = 0;
    timers[i].started= 0;
    timers[i].flag    = 0;
  }
}

void swtSetPeriod( int id, unsigned long int p ) {
//  assert( id >= 0 && id < MAX_TIMERS );
  timers[id].period  = p;
  timers[id].counter = p;
  timers[id].started = 1;
  timers[id].flag    = 0;  
}

void swtResetFlag( int id ){
//  assert( id >= 0 && id < MAX_TIMERS );
  timers[id].flag    = 0;  
}

void swtStart( int id ){
//  assert( id >= 0 && id < MAX_TIMERS );
  timers[id].started = 1;
}

void swtStop( int id ) {
//  assert( id >= 0 && id < MAX_TIMERS );
  timers[id].started = 0;
}

int swtExpired( int id ){
//  assert( id >= 0 && id < MAX_TIMERS );
  return timers[id].flag;
}

void swtUpdate( void ){
  int i;
  for( i = 0; i < MAX_TIMERS; i++ ) {
    if( timers[i].started == 1 ) {
      if( timers[i].counter == 0 ) {
        timers[i].counter = timers[i].period;
        timers[i].flag = 1;
      } else {
         timers[i].counter--;
      }
    }
  }
}

