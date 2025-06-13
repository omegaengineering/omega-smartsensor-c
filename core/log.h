#ifndef SMARTSENSOR_LOG_H
#define SMARTSENSOR_LOG_H

#if __linux__
#include <stdio.h>
#else
#define S_LOG 0         // disable log
#endif

#ifndef S_LOG
#define S_LOG   1
#endif


#if S_LOG && __linux__
#define s_log(fmt,...)        do {     fprintf(stdout, fmt, ##__VA_ARGS__); \
                                       fflush(stdout); } \
                                   while (0)
#else
#define s_log(fmt,...)
#endif

#endif //SMARTSENSOR_LOG_H
