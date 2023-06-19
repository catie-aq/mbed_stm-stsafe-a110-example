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
static DigitalOut synchro(P1_DIO13);
static STSafeA110 stsafea110;
static uint8_t status_code;
static uint8_t read_key[2 * STSAFEA_HOST_KEY_LENGTH];
static uint8_t Host_MAC_Cipher_Key[2 * STSAFEA_HOST_KEY_LENGTH] = {
        0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,    /* Host MAC key */
        0x11,0x11,0x22,0x22,0x33,0x33,0x44,0x44,0x55,0x55,0x66,0x66,0x77,0x77,0x88,0x88     /* Host cipher key */
    };

void print_byte(uint8_t *key, int size_of_key)
{
    for (int i = 0; i < size_of_key; i++) {
        printf("0x%02X ", key[i]);
    }
    printf("\n");
}

int main()
{
    synchro = 0;
    /* Create STSAFE-A's handle */
    status_code = stsafea110.init();
    if (status_code == STSAFEA_OK) {
        printf("\n\r\n\r\n\r\n\rSTSAFE-A110 initialized successfully\n\r");
    } else {
        printf("\n\r\n\r\n\r** STSAFE-A110 NOT initialized. ** \n\r");
        return (-1);
    }

    // Write data
    uint8_t data[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
    synchro = 1;
    printf("Write: %d\n", stsafea110.update_data_partition(2, data, sizeof(data)));
    synchro = 0;
    
    for (unsigned int i = 0; i < sizeof(data); i++) {
        printf("0x%X ", data[i]);
    }
    printf("\n");

    // Read data
    uint8_t read_data[10];
    synchro = 1;
    printf("Read: %d\n", stsafea110.read_data_partition(2, read_data, sizeof(read_data)));
    synchro = 0;

    for (unsigned int i = 0; i < sizeof(read_data); i++) {
        printf("0x%X ", read_data[i]);
    }
    printf("\n");

    led1 = 0;
    status_code = stsafea110.pairing(Host_MAC_Cipher_Key);
    if (status_code == STSAFEA_OK) {
         led1 = 1;
#ifdef MBED_CONF_STM_STSAFE_A110_DEBUG_LOG
        printf("\n\r\n\r ------ Pairing succeeded ------ \n\r");
#endif /* MBED_CONF_STM_STSAFE_A110_DEBUG_LOG */
    } else {
#ifdef MBED_CONF_STM_STSAFE_A110_DEBUG_LOG
        printf("\n\r\n\r ----- Pairing Failed -----\n\r");
#endif /* MBED_CONF_STM_STSAFE_A110_DEBUG_LOG */
        int i = 0;
        for (i = 0; i < 10; i++) {
            led1 = !led1;
            ThisThread::sleep_for(HALF_PERIOD);
        }
    }
    printf("Read partition 0: %d\n", stsafea110.read_data_partition(0, read_key, sizeof(read_key)));
    printf("Is Key protected ? [");
    if (read_key != Host_MAC_Cipher_Key) {
        printf("True]\n");
    } else {
        printf("False]\n");
    }
    print_byte(read_key, sizeof(read_key));

    // Write data
    synchro = 1;
    printf("Write: %d\n", stsafea110.update_data_partition(2, data, sizeof(data)));
    synchro = 0;
    
    for (unsigned int i = 0; i < sizeof(data); i++) {
        printf("0x%X ", data[i]);
    }
    printf("\n");

    // Read data
    synchro = 1;
    printf("Read: %d\n", stsafea110.read_data_partition(2, read_data, sizeof(read_data)));
    synchro = 0;

    for (unsigned int i = 0; i < sizeof(read_data); i++) {
        printf("0x%X ", read_data[i]);
    }
    printf("\n");


    /* Infinite loop */
//while (status_code == STSAFEA_OK)
//{
//    led1 = 0;
//    int i = 0;
//
//    if (stsafea110.wrap_unwrap(STSAFEA_KEY_SLOT_0) == 0) {
//        led1 = 1;
//    } else {
//        for (i = 0; i < 10; i++) {
//            led1 = !led1;
//            ThisThread::sleep_for(HALF_PERIOD);
//        }
//    }
//
//    /* Wait for push BUTTON_USER */
//        while ((uint8_t)BSP_PB_GetState(BUTTON_USER) == BUTTON_RELEASED) {};
//}

    return (0);

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
