# _Simple 1-Wire library for the ESP32 (IDF v5.0)_

This library for the ESP32 aims to be both high performance and easy to use. It implements the same API as my [example for the RPi Pico (RP2040)](https://github.com/raspberrypi/pico-examples/tree/develop/pio/onewire).

Internally it is based on the ESP's [RMT peripheral](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html). Whilst originally designed as an IR tranceiver it provides a good general purpose hardware bitstream encoder/decoder. Using the RMT is far more efficient and accurate than bit-bashing a GPIO.

> NOTE: the library uses the new RMT driver **which requires [ESP-IDF Release v5.0](https://www.espressif.com/en/news/ESP-IDFv5) or above.**

## Example program

The quickest way to understand a library is to see it in action. So this repository shows a simple example to take readings from several [DS18B20](https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf) temperature sensors.

By default the example uses GPIO_4 for the 1-Wire bus but this can be easily changed (see `main.c`).

The example is in the `main` folder, and the library itself is in the `components` folder. 

The project can be built using idy.py or VSCode; instructions are beyond the scope of this README, but are well explained in the ESP [Getting Started](https://docs.espressif.com/projects/esp-idf/en/v5.0.1/esp32/get-started/index.html) documentation. 

## File layout


```
.
├── CMakeLists.txt
├── README.md                               the file you are reading
├── components
│   └── esp32-onewire                       the 1-wire library
│       ├── CMakeLists.txt
│       ├── include
│       │   ├── onewire.h                   (include this header file to use the library)
│       │   ├── onewire_symbols.h
│       │   └── onewire_timings.h
│       ├── ow_init.c
│       ├── ow_read.c
│       ├── ow_reset.c
│       ├── ow_romsearch.c
│       └── ow_send.c
├── main
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── ds18b20.h                       function command codes for the DS18B20
│   │   └── ow_rom.h                        generic 1-Wire ROM command codes
│   └── main.c                              example program to read DS18B20 sensors
```


*1-Wire(R)  is a trademark of Maxim Integrated Products, Inc.*