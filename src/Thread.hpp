#ifndef _WIN32
#include <pthread.h>
#define THREAD_TYPE pthread_t
#define THREAD_CREATE(variable, function, args) pthread_create(&variable, NULL, function, args);
#define THREAD_JOIN(variable) pthread_join(variable, NULL);
#else
#include <thread>
#define THREAD_TYPE std::thread
#define THREAD_CREATE(variable, function, args) variable = std::thread(function, args);
#define THREAD_JOIN(variable) variable.join();
#endif