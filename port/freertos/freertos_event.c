#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "port.h"

#define EVQ_MAX_ITEM        10
#define pdMSTOTICKS(ms)     ((ms * configTICK_RATE_HZ * 1ULL) / 1000)
#define EVQ_MAX_WAIT        pdMSTOTICKS(100)


static QueueHandle_t g_evq;

int evq_init()
{
    g_evq = xQueueCreate(EVQ_MAX_ITEM, sizeof(port_event_t));
    if (g_evq)
        return E_OK;
    else
        return E_NO_MEM;
}

int evq_send(port_event_t* event)
{
    if (xQueueSend( g_evq,event,(TickType_t) EVQ_MAX_WAIT ) == pdPASS)
        return E_OK;
    else
        return E_FULL;
}

int evq_recv(port_event_t* event)
{
    if (xQueueReceive( g_evq, event, portMAX_DELAY ) == pdPASS)
        return E_OK;
    else
        return E_EMPTY;
}

int evq_deinit()
{
    vQueueDelete(g_evq);
    return E_OK;
}

