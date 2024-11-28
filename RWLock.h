#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;      
    pthread_cond_t readersOK;  
    int readers;                
    int writers;                
} RWLock;

void InitRWLock(RWLock *lock);
void DestroyRWLock(RWLock *lock);

void ReadLock(RWLock *lock);
void ReadUnlock(RWLock *lock); 
void WriteLock(RWLock *lock);
void WriteUnLock(RWLock *lock);

#endif