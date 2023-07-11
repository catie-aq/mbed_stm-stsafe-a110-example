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
static InterruptIn button(BUTTON1);

static Thread thread;
static EventQueue queue;

static STSafeA110 stsafea110;
static uint8_t status_code;
static uint8_t Host_MAC_Cipher_Key[2 * STSAFEA_HOST_KEY_LENGTH] = {
        0x7A, 0xE2, 0xD7, 0x31, 0x18, 0x58, 0x7F, 0x59, 0x6E, 0x05, 0xF5, 0x8E, 0x81, 0x1D, 0x54, 0xE7,    /* Host MAC key */
        0xF8, 0x5C, 0xA8, 0x37, 0xBD, 0xF3, 0xF7, 0xCC, 0x5B, 0xC2, 0xE7, 0x7B, 0x4C, 0xC1, 0xBA, 0x6E     /* Host cipher key */
    };

void button_routine() {

    // Write data
    uint8_t data[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
    synchro = 1;
    if (stsafea110.update_data_partition(2, data, sizeof(data)) != STSAFEA_OK) {
        return;
    }
    synchro = 0;

    // Read data
    uint8_t read_data[10];
    synchro = 1;
    if (stsafea110.read_data_partition(2, read_data, sizeof(read_data)) != STSAFEA_OK) {
        return;
    }
    synchro = 0;

    ThisThread::sleep_for(100ms);

    if (!stsafea110.paired()) {
        printf("-------- STSafe-A110 pairing! --------\n");
        stsafea110.pairing(Host_MAC_Cipher_Key);
        printf("--------- STSafe-A110 paired ---------\n");
    }
    else {
        printf("----- STSafe-A110 Already paired -----\n");
    }

}

void button_interrupt() {
    queue.call(button_routine);
}

int main()
{
    synchro = 0;
    // StSafe Init
    printf("\n\n\n----- STSafe-A110 initialization -----\n");
    stsafea110.init();
    if (status_code != STSAFEA_OK) {
        return (1);
    }
    printf("------ STSafe-A110 initialized! ------\n");

    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    button.rise(button_interrupt);

    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(HALF_PERIOD);
    }

    return (0);
}
