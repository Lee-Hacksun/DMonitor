#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <pthread.h>

extern pthread_mutex_t  g_client_list_lock;
extern pthread_mutex_t  g_sensor_lock;
extern pthread_mutex_t  g_progress_lock;
extern pthread_mutex_t g_log_file;

#endif