//
// Created by dev on 5/23/19.
//

#ifndef OMEGA_SMARTSENSOR_C_LOG_H
#define OMEGA_SMARTSENSOR_C_LOG_H

#include <stdio.h>

#ifndef NDEBUG

#define LOG_LEVEL_NONE  0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_DEBUG 2


#define s19_log_create(name, level)    static const char log_module[] = name; static int log_level = level
#define s19_log_info(fmt,...)   do { if (log_level >= LOG_LEVEL_INFO)  {printf("[%5s][INFO]: " fmt,log_module, ##__VA_ARGS__); fflush(stdout);} } while(0)
#define s19_log_dbg(fmt,...)    do { if (log_level >= LOG_LEVEL_DEBUG) {printf("[%5s][ DBG]: " fmt,log_module, ##__VA_ARGS__); fflush(stdout);} } while(0)
#define s19_log_warn(fmt,...)   do { if (1) {printf("[%5s][WARN]: " fmt,log_module, ##__VA_ARGS__); fflush(stdout);} } while(0)

#else
#define s19_log_create(name)
#define s19_log_info(fmt,...)
#define s19_log_dbg(fmt,...)
#define s19_log_warn(fmt,...)

#endif //NDEBUG


#endif //OMEGA_SMARTSENSOR_C_LOG_H
