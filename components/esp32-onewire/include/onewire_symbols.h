#include "onewire_timings.h"

#define OW_BUS_ASSERTED 1
#define OW_BUS_RELEASED 0

#define OW_SYMBOL_0 { \
    .level0 = OW_BUS_ASSERTED, \
    .duration0 = OW_TIMING_PARAM_C, \
    .level1 = OW_BUS_RELEASED, \
    .duration1 = OW_TIMING_PARAM_D }

#define OW_SYMBOL_1 { \
    .level0 = OW_BUS_ASSERTED, \
    .duration0 = OW_TIMING_PARAM_A, \
    .level1 = OW_BUS_RELEASED, \
    .duration1 = OW_TIMING_PARAM_B }

#define OW_SYMBOL_RESET { \
    .level0 = OW_BUS_ASSERTED, \
    .duration0 = OW_TIMING_PARAM_H, \
    .level1 = OW_BUS_RELEASED, \
    .duration1 = OW_TIMING_PARAM_I + OW_TIMING_PARAM_J }
