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
static uint8_t status_code;

int main()
{
    /* Create STSAFE-A's handle */
    status_code = stsafea110.init();
    if (status_code == STSAFEA_OK) {
        printf("\n\r\n\r\n\r\n\rSTSAFE-A110 initialized successfully\n\r");
    } else {
        printf("\n\r\n\r\n\r** STSAFE-A110 NOT initialized. ** \n\r");
        return (-1);
    }

    /* Infinite loop */
    //while (status_code == STSAFEA_OK)
    //{
        //led1 = 0;
        //int i = 0;
//
        //if (stsafea110.wrap_unwrap(STSAFEA_KEY_SLOT_0) == 0) {
        //    led1 = 1;
        //} else {
        //    for (i = 0; i < 10; i++) {
        //        led1 = !led1;
        //        ThisThread::sleep_for(HALF_PERIOD);
        //    }
        //}

        /* Wait for push BUTTON_USER */
//        while ((uint8_t)BSP_PB_GetState(BUTTON_USER) == BUTTON_RELEASED) {};
    //}

    led1 = 0;
    status_code = stsafea110.pairing();
    if (status_code == STSAFEA_OK) {
         led1 = 1;
#ifdef MBED_CONF_STM_STSAFE_A110_DEBUG_LOG
        printf("\n\r\n\rPairing succeeded \n\r");
#endif /* MBED_CONF_STM_STSAFE_A110_DEBUG_LOG */
    } else {
#ifdef MBED_CONF_STM_STSAFE_A110_DEBUG_LOG
        printf("\n\r\n\rPairing Failed \n\r");
#endif /* MBED_CONF_STM_STSAFE_A110_DEBUG_LOG */
        int i = 0;
        for (i = 0; i < 10; i++) {
            led1 = !led1;
            ThisThread::sleep_for(HALF_PERIOD);
        }
    }

    return (0);

//    // Write data
//    uint8_t data[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
//    printf("Write: %d\n", stsafea110.update_data_partition(2, data, sizeof(data)));
//
//    // Read data
//    uint8_t read_data[10];
//    printf("Read: %d\n", stsafea110.read_data_partition(2, read_data, sizeof(read_data)));
//
//    for (unsigned int i = 0; i < sizeof(read_data); i++) {
//        printf("0x%X ", read_data[i]);
//    }
//
//    printf("\n");
//
//    while (true) {
//        led1 = !led1;
//        if (led1) {
//            printf("Alive!\n");
//        }
//        ThisThread::sleep_for(HALF_PERIOD);
//    }
//
//    return 0;
}
