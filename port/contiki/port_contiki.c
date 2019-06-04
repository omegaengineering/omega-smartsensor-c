#include <assert.h>
#include <stdio.h>
#include "contiki.h"
#include "smartsensor_private.h"
#include "common/log.h"
#include "hw/ti/bus_ti_i2c.h"
#include "hw/ti/bus_ti_uart_modbus.h"

typedef struct {
    PIN_State               pin_state;
    PIN_Handle              pin_handle;
    PIN_Id                  pin_id;
} ti_gpio_t;

ti_gpio_t gpio_intr;
ti_gpio_t * intr = &gpio_intr;

bus_ti_i2c_cfg_t my_config = {
        .periph_idx = 0,
        .bit_rate = I2C_100kHz,
        .sda_pin = 4,
        .clk_pin = 5,
        .slave_addr = SMARTSENSOR_I2C_ADDR
};

const bus_new_dr    port_bus_i2c_new = bus_ti_i2c_new;
const void*         port_bus_i2c_cfg = &my_config;
const bus_new_dr    port_bus_modbus_new = bus_ti_uart_modbus_new;
const void*         port_bus_modbus_cfg = (const void*)NULL;


s19_log_create("PORT", LOG_LEVEL_DEBUG);

PROCESS(port_task, "Port Task");

process_event_t     interrupt_os_evt;
static uint8_t      port_task_ready;


PROCESS_THREAD(port_task, event, pv_Data)
{
    static sensor_t * sensor = NULL;

    PROCESS_BEGIN();

    if (event == PROCESS_EVENT_INIT)
    {
        sensor = pv_Data;
        s19_log_info("Sensor init %d\n", sensor == NULL);
        port_task_ready = true;
    }

    while (1)
    {
        PROCESS_WAIT_EVENT();

        if (event == interrupt_os_evt && sensor != NULL)
            hard_intr_triggered(sensor);
    }

    port_task_ready = false;
    s19_log_dbg("Port task terminated\n");

    PROCESS_END();
}

static void ti_gpio_intr_cb (PIN_Handle handle, PIN_Id pinId)
{
    int status;
    if (port_task_ready)
    {
        /// Defer interrupt event to be handled by port task
        /// Do not post synch event in this cooperative scheduling env
        status = process_post(&port_task, interrupt_os_evt, NULL);
        assert(status == PROCESS_ERR_OK);
    }
}

int port_intr_init(sensor_t *sensor)
{
    PIN_Status status;
    intr->pin_id = sensor->gpio_pin;
    assert(intr->pin_id == 24);
    PIN_Config pin_config_list[] = {intr->pin_id , PIN_TERMINATE};

    intr->pin_handle = PIN_open(&intr->pin_state, pin_config_list);
    if (intr->pin_handle == NULL)
        return E_BUS_OPERATION;

    if ((status = PIN_setConfig(intr->pin_handle, PIN_BM_INPUT_EN|PIN_BM_GPIO_OUTPUT_EN|PIN_BM_PULLING,
                  intr->pin_id|PIN_INPUT_EN|PIN_GPIO_OUTPUT_DIS|PIN_PULLUP)) != PIN_SUCCESS)
        return E_BUS_OPERATION;

    if ((status = PIN_registerIntCb(intr->pin_handle, ti_gpio_intr_cb)) != PIN_SUCCESS)
        return E_BUS_OPERATION;

    if ((status = PIN_setInterrupt(intr->pin_handle, intr->pin_id|PIN_IRQ_NEGEDGE)) != PIN_SUCCESS)
        return E_BUS_OPERATION;

    return E_OK;
}



int port_platform_init(sensor_t *sensor)
{
    interrupt_os_evt = process_alloc_event();

    process_start (&port_task, sensor);

    return E_OK;
}

int port_platform_exit(sensor_t *sensor)
{
    int status;
    process_exit(&port_task);
    status = PIN_setInterrupt(intr->pin_handle, intr->pin_id|PIN_IRQ_DIS);
    PIN_close(intr->pin_handle);
    (void) status;
    return E_OK;
}
