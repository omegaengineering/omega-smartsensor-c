/*!********************************************************************************************
  @file     errors.h

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

#ifndef OMEGA_SMARTSENSOR_C_ERRORS_H
#define OMEGA_SMARTSENSOR_C_ERRORS_H

typedef enum {
    E_OK                = 0x00000,
    E_BUS_UNAVAILABLE   = 0xf0000,
    E_BUS_OPERATION,
    E_API_ERROR,
    E_BUFFER_MEM_SIZE,
    E_INVALID_PARAM,
    E_NO_MEM,
    E_NOT_SUPPORTED,
    E_TRY_AGAIN
} error_t;

#endif //OMEGA_SMARTSENSOR_C_ERRORS_H
