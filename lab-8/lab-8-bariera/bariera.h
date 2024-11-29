#ifndef BARRIER_H
#define BARRIER_H

void bariera_init(int threads_count);
void bariera(void);                     // wywolanie bariery przez watek
void bariera_destroy(void);

#endif