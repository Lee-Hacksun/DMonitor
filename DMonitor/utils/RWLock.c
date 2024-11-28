#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "RWLock.h"

void InitRWLock(RWLock *lock) {
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->readersOK, NULL);
    lock->readers = 0;
    lock->writers = 0;
}

void DestroyRWLock(RWLock *lock) {
    pthread_mutex_destroy(&lock->mutex);
    pthread_cond_destroy(&lock->readersOK);
}

void ReadLock(RWLock *lock) {
    pthread_mutex_lock(&lock->mutex);
    while (lock->writers > 0) {
        pthread_cond_wait(&lock->readersOK, &lock->mutex);
    }
    lock->readers++;
    pthread_mutex_unlock(&lock->mutex);
}

void ReadUnlock(RWLock *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->readers--;
    if (lock->readers == 0) {
        pthread_cond_signal(&lock->readersOK);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void WriteLock(RWLock *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->writers++;
    while (lock->readers > 0) {
        pthread_cond_wait(&lock->readersOK, &lock->mutex);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void WriteUnLock(RWLock *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->writers--;
    pthread_cond_broadcast(&lock->readersOK);
    pthread_mutex_unlock(&lock->mutex);
}