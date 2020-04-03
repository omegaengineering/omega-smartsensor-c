#include <stdint.h>
#include <pthread.h>

typedef enum {
    EV_NONE,
    EV_SENSOR_READ,
    EV_SENSOR_WRITE,
} ev_t;

typedef struct {
    ev_t        event;
    uint8_t*    data;
    uint16_t    data_len;
} ev_data_t;

typedef struct _ev_queue {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    uint16_t        size;
    uint16_t        head;
    uint16_t        tail;
    ev_data_t**     queue;
} ev_queue_t;

int evq_init();

int evq_enqueue(ev_t event, uint8_t* data, uint16_t data_len);

int evq_get(ev_t event);

int evq_close();