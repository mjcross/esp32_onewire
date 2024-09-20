/**
 * Copyright (c) 2023 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
**/

#include "onewire.h"

int ow_romsearch (OW *ow, uint64_t *romcodes, int maxdevs, unsigned int command) {
    int index;
    uint64_t romcode = 0ull;
    int branch_point;
    int next_branch_point = -1;
    int num_found = 0;
    bool finished = false;

    while (finished == false && (maxdevs == 0 || num_found < maxdevs )) {
        finished = true;
        branch_point = next_branch_point;
        if (ow_reset (ow) == false) {
            num_found = 0;     // no slaves present
            finished = true;
            break;
        }
        ow_send (ow, command);
        for (index = 0; index < 64; index += 1) {   // determine romcode bits 0..63 (see ref)
            unsigned int a = ow_read_bit (ow);
            unsigned int b = ow_read_bit (ow);
            if (a == 0 && b == 0) {         // (a, b) = (0, 0)
                if (index == branch_point) {
                    ow_send_bit (ow, 1);
                    romcode |= (1ull << index);
                } else {
                    if (index > branch_point || (romcode & (1ull << index)) == 0) {
                        ow_send_bit (ow, 0);
                        finished = false;
                        romcode &= ~(1ull << index);
                        next_branch_point = index;
                    } else {                // index < branch_point or romcode[index] = 1
                        ow_send_bit (ow, 1);
                    }
                }
            } else if (a != 0 && b != 0) {  // (a, b) = (1, 1) error (e.g. device disconnected)
                num_found = -2;             // function will return -1
                finished = true;
                break;                      // terminate `for` loop
            } else {
                if (a == 0) {               // (a, b) = (0, 1) or (1, 0)
                    ow_send_bit (ow, 0);
                    romcode &= ~(1ull << index);
                } else {
                    ow_send_bit (ow, 1);
                    romcode |= (1ull << index);
                }
            }
        }                                   // end of `for` loop

        if (romcodes != NULL) {
            romcodes[num_found] = romcode;  // store the romcode
        }
        num_found += 1;
    }                                       // end of `while` loop

    return num_found;
}
