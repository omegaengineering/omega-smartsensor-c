#ifndef SMARTSENSOR_PORT_FREERTOS_H
#define SMARTSENSOR_PORT_FREERTOS_H

int evq_init();
int evq_send(port_event_t* event);
int evq_recv(port_event_t* event);
int evq_deinit();

void port_task_delay(uint32_t ms);

#endif //SMARTSENSOR_PORT_FREERTOS_H
