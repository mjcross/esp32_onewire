#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "onewire.h"
#include "ow_rom.h"
#include "ds18b20.h"

#define GPIO 4
#define MAXDEVS 10

void app_main(void)
{
    OW ow;
    ow_init(&ow, GPIO);
    if (ow_reset (&ow)) {
        puts ("slave(s) present");

        // scan bus for slaves
        uint64_t romcode[MAXDEVS];
        int num_devs = ow_romsearch (&ow, romcode, MAXDEVS, OW_SEARCH_ROM);
        printf("Found %d devices\n", num_devs);      
        for (int i = 0; i < num_devs; i += 1) {
            printf("\t%d: 0x%llx\n", i, romcode[i]);
        }
        putchar ('\n');

        // get temperature readings
        while (num_devs > 0) {
            // start temperature conversion in parallel on all devices
            // (see ds18b20 datasheet)
            ow_reset (&ow);
            ow_send (&ow, OW_SKIP_ROM);
            ow_send (&ow, DS18B20_CONVERT_T);

            // wait for the conversions to finish
            while (ow_read(&ow) == 0);

            // read the result from each device
            for (int i = 0; i < num_devs; i += 1) {
                ow_reset (&ow);
                ow_send (&ow, OW_MATCH_ROM);
                for (int b = 0; b < 64; b += 8) {
                    ow_send (&ow, romcode[i] >> b);
                }
                ow_send (&ow, DS18B20_READ_SCRATCHPAD);
                int16_t temp = 0;
                temp = ow_read (&ow) | (ow_read (&ow) << 8);
                printf ("\t%d: %f", i, temp / 16.0);
                fflush (stdout);
            }
            putchar ('\n');
        }
    } else {
        puts ("no slave");
    }
    puts ("done");
    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
