#include <assert.h>
#include <stdio.h>
#include "platform/timer.h"
#include "common/errors.h"

void callback11(void* arg)
{
    printf("Callback 11 \n");
}

void callback22(void* arg)
{
    printf("Callback 22 \n");
}

int main()
{
    int ret;
    s19_timer_t *timer1;
    ret = s19_timer_create(&timer1);
    assert(ret == E_OK);
    s19_timer_entry_t * entry11;
    ret = s19_timer_entry_add(timer1, &entry11);
    assert(ret == E_OK);
    ret = s19_timer_entry_set_period(entry11, 1);
    assert(ret == E_OK);
    ret = s19_timer_entry_set_callback(entry11, callback11);

    s19_timer_entry_t * entry22;
    ret = s19_timer_entry_add(timer1, &entry22);
    assert(ret == E_OK);
    ret = s19_timer_entry_set_period(entry22, 3);
    assert(ret == E_OK);
    ret = s19_timer_entry_set_callback(entry22, callback22);


    getchar();
}