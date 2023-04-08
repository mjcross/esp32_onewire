#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"

#define OW_TX_MEM_BLOCK_SYMBOLS 64
#define OW_TX_QUEUE_DEPTH 4
#define OW_MAX_READ_BITS 64
#define OW_RX_MEM_BLOCK_SYMBOLS (OW_MAX_READ_BITS + 2)
#define OW_RX_MIN_NS 1000
#define OW_RMT_TIMEOUT_MS 1000
#define OW_RX_MARGIN_US 3

typedef struct {
    rmt_channel_handle_t tx_channel;
    rmt_channel_handle_t rx_channel;
    rmt_encoder_handle_t copy_encoder;
    rmt_encoder_handle_t bytes_encoder;
    rmt_symbol_word_t *rx_buffer;
    size_t rx_buflen;
    QueueHandle_t rx_queue;
} OW;

bool ow_init (OW *ow, int gpio);
void ow_deinit (OW *ow);
void ow_send (OW *ow, uint data);
void ow_send_bit (OW *ow, uint data);
uint8_t ow_read (OW *ow);
bool ow_read_bit (OW *ow);
bool ow_reset (OW *ow);
int ow_romsearch (OW *ow, uint64_t *romcodes, int maxdevs, uint command);
