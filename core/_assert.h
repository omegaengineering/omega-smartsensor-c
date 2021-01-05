#ifndef SMARTSENSOR_ASSERT_H
#define SMARTSENSOR_ASSERT_H

/* For C++: */
#ifdef __cplusplus
#ifndef _Static_assert
        #define _Static_assert static_assert /* `static_assert` is part of C++11 or later */
    #endif
#endif
/* Now for gcc (C) (and C++, given the define above): */
#define STATIC_ASSERT(test_for_true) _Static_assert((test_for_true), "(" #test_for_true ") failed")


#endif //SMARTSENSOR_ASSERT_H
