#ifndef _SWT_H_
#define _SWT_H_

#define MAX_TIMERS	16

void swtInit(void);
void swtSetPeriod( int id, unsigned long int p );
void swtResetFlag( int id );
void swtStart( int id );
void swtStop( int id );
int  swtExpired( int id );
void swtUpdate(void);

#endif
