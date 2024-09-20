/**
 * Copyright (c) 2023 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
**/

#include "esp_log.h"
#include "onewire.h"
#include "onewire_symbols.h"


static const rmt_symbol_word_t symbol_0 = OW_SYMBOL_0;
static const rmt_symbol_word_t symbol_1 = OW_SYMBOL_1;

static const rmt_transmit_config_t tx_config = {
    .flags.eot_level = OW_BUS_RELEASED
};


void ow_send (OW *ow, unsigned int data) {
    rmt_transmit (ow->tx_channel, ow->bytes_encoder, &data, 1, &tx_config);
        if (rmt_tx_wait_all_done (ow->tx_channel, OW_RMT_TIMEOUT_MS) != ESP_OK) {
        ESP_LOGE (__func__, "tx timeout");
    }
}


void ow_send_bit (OW *ow, unsigned int data) {
    const rmt_symbol_word_t *ps = &symbol_0;
    if (data != 0) {
        ps = &symbol_1;
    }
    rmt_transmit (ow->tx_channel, ow->copy_encoder, ps, sizeof (rmt_symbol_word_t), &tx_config);
    if (rmt_tx_wait_all_done (ow->tx_channel, OW_RMT_TIMEOUT_MS) != ESP_OK) {
        ESP_LOGE (__func__, "tx timeout");
    }
}
