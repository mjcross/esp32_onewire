/**
 * Copyright (c) 2023 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
**/

#include "esp_log.h"
#include "onewire.h"
#include "onewire_symbols.h"

static const rmt_receive_config_t rx_config = {
    .signal_range_min_ns = OW_RX_MIN_NS,
    .signal_range_max_ns = (OW_TIMING_PARAM_A + OW_TIMING_PARAM_B) * 1000
};


static uint8_t _parse_bit_symbols(size_t num_symbols, rmt_symbol_word_t *symbol) {
    uint8_t bits = 0;
    int bit_count = 0;
    int symbol_count = 0;
    while (symbol_count < num_symbols && bit_count < 8) {
        if (symbol->duration0 <= OW_TIMING_PARAM_A + OW_RX_MARGIN_US &&
            (symbol->duration1 == 0 || symbol->duration1 >= OW_TIMING_PARAM_E)) {
                bits |= (1 << bit_count);
                bit_count += 1;
            }
        else if (symbol->duration0 >= OW_TIMING_PARAM_A + OW_TIMING_PARAM_E) {
            bit_count += 1;
        }
        symbol_count += 1;
        symbol += 1;
    }
    return bits;
}


uint8_t ow_read (OW *ow) {
    rmt_rx_done_event_data_t evt;
    rmt_receive (ow->rx_channel, ow->rx_buffer, ow->rx_buflen, &rx_config);
    ow_send (ow, 0xff);
    if (xQueueReceive (ow->rx_queue, &evt, pdMS_TO_TICKS(OW_RMT_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE (__func__, "rx timeout");
        return false;
    }
    return _parse_bit_symbols (evt.num_symbols, evt.received_symbols);
}


bool ow_read_bit (OW *ow) {
    rmt_rx_done_event_data_t evt;
    rmt_receive (ow->rx_channel, ow->rx_buffer, ow->rx_buflen, &rx_config);
    ow_send_bit (ow, 1);
    if (xQueueReceive (ow->rx_queue, &evt, pdMS_TO_TICKS(OW_RMT_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE (__func__, "rx timeout");
        return false;
    }
    return (_parse_bit_symbols (evt.num_symbols, evt.received_symbols) & 0x01);
}