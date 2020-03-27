#include "port_linux.h"

int port_platform_init(int instance)
{
    int ret;
    if ((ret = port_bus_open(instance)) != E_OK)
        return ret;
    return E_OK;
}

int port_platform_deinit(int instance)
{
    (void) port_bus_close(instance);
    return E_OK;
}