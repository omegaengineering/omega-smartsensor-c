#include "port.h"

sensor_t* port_get_sensor(const port_t* p)
{
    return p->sensor;
}
