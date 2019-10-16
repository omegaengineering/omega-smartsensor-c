/*!********************************************************************************************
  @file     log.h

  @copyright
            Copyright (c) 2019, Omega Engineering Inc.

            Permission is hereby granted, free of charge, to any person obtaining
            a copy of this software and associated documentation files (the
            'Software'), to deal in the Software without restriction, including
            without limitation the rights to use, copy, modify, merge, publish,
            distribute, sublicense, and/or sell copies of the Software, and to
            permit persons to whom the Software is furnished to do so, subject to
            the following conditions:

            The above copyright notice and this permission notice shall be
            included in all copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
            IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
            CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
            TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
            SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  @author   Binh Dinh
  @date     June 5th, 2019
  @details


***********************************************************************************************/
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
#define s19_log_create(name, level)
#define s19_log_info(fmt,...)
#define s19_log_dbg(fmt,...)
#define s19_log_warn(fmt,...)

#endif //NDEBUG


#endif //OMEGA_SMARTSENSOR_C_LOG_H
