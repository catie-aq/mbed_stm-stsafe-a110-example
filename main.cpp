/*
 * Copyright (c) 2023, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "stsafe_a110.h"

using namespace sixtron;

namespace {
#define HALF_PERIOD 500ms
}

static DigitalOut led1(LED1);
static STSafeA110 stsafea110;

int main()
{
    // Initialize STSafe-A110
    if (stsafea110.init()) {
        printf("Failed to init STSafe-A110");
    }

    // Write data
    uint8_t data[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
    printf("Write: %d\n", stsafea110.update_data_partition(2, data, sizeof(data)));

    // Read data
    uint8_t read_data[10];
    printf("Read: %d\n", stsafea110.read_data_partition(2, read_data, sizeof(read_data)));

    for (unsigned int i = 0; i < sizeof(read_data); i++) {
        printf("0x%X ", read_data[i]);
    }

    printf("\n");

    while (true) {
        led1 = !led1;
        if (led1) {
            printf("Alive!\n");
        }
        ThisThread::sleep_for(HALF_PERIOD);
    }

    return 0;
}
