#include "port.h"

sensor_t* port_get_sensor(const port_t* p)
{
    return p->sensor;
}

sensor_bus_type_t port_get_bus_type(const port_t* p)
{
    return p->bus_type;
}
