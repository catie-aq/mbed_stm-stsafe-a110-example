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
        0x7A, 0xE2, 0xD7, 0x31, 0x18, 0x58, 0x7F, 0x59, 0x6E, 0x05, 0xF5, 0x8E, 0x81, 0x1D, 0x54, 0xE7,    /* Host MAC key */
        0xF8, 0x5C, 0xA8, 0x37, 0xBD, 0xF3, 0xF7, 0xCC, 0x5B, 0xC2, 0xE7, 0x7B, 0x4C, 0xC1, 0xBA, 0x6E     /* Host cipher key */
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
    // StSafe Init
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

    // Pairing
    led1 = 0;
    status_code = stsafea110.pairing(Host_MAC_Cipher_Key);
    if (status_code == STSAFEA_OK) {
         led1 = 1;
        printf("\n\r\n\r ------ Pairing succeeded ------ \n\r");
    } else {
        printf("\n\r\n\r ----- Pairing Failed -----\n\r");
        int i = 0;
        for (i = 0; i < 10; i++) {
            led1 = !led1;
            ThisThread::sleep_for(HALF_PERIOD);
        }
    }
    //printf("Read partition 0: %d\n", stsafea110.read_data_partition(0, read_key, sizeof(read_key)));
    //printf("Is Key protected ? [");
    //if (read_key != Host_MAC_Cipher_Key) {
    //    printf("True]\n");
    //} else {
    //    printf("False]\n");
    //}
    //print_byte(read_key, sizeof(read_key));

    return (0);
}
