/*
 * Copyright (c) 2023, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "BlockDevice.h"
#include "FlashIAPBlockDevice.h"
#include "mbed.h"
#include "stsafe_a110.h"

using namespace sixtron;

#define FLASHIAP_ADDRESS_H753ZI 0x08100000
#define FLASHIAP_ADDRESS_L4A6RG 0x08080000
#define FLASHIAP_SIZE 0x1000

namespace {
#define HALF_PERIOD 500ms
}

// Peripherals
static DigitalOut led1(LED1);
static STSafeA110 stsafea110;
// Create flash IAP block device
BlockDevice *bd = new FlashIAPBlockDevice(FLASHIAP_ADDRESS_L4A6RG, FLASHIAP_SIZE);
// Variable
bool pairing_status = 0;
bool wrap_unwrap_status = 0;
static uint8_t read_key[32];
static uint8_t Host_MAC_Cipher_Key[2U * STSAFEA_HOST_KEY_LENGTH] = {
    0x00,
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66,
    0x77,
    0x88,
    0x99,
    0xAA,
    0xBB,
    0xCC,
    0xDD,
    0xEE,
    0xFF, /* Host MAC key */
    0x11,
    0x11,
    0x22,
    0x22,
    0x33,
    0x33,
    0x44,
    0x44,
    0x55,
    0x55,
    0x66,
    0x66,
    0x77,
    0x77,
    0x88,
    0x88 /* Host cipher key */
};

int read_configuration()
{
    if (bd->read(read_key, 0, sizeof(read_key)) != 0) {
        return -1;
    }
    return 0;
}

int write_configuration(uint8_t *key)
{
    if (bd->erase(0, bd->size()) != 0) {
        return -1;
    }

    if (bd->program(key, 0, bd->get_erase_size()) != 0) {
        return -2;
    }

    if (bd->sync() != 0) {
        return -3;
    }

    return 0;
}

void flash_init(void)
{

    // Initialize the flash IAP block device and print the memory layout
    bd->init();
    printf("\n\nFlash block device size: %llu\n", bd->size());
    printf("Flash block device read size: %llu\n", bd->get_read_size());
    printf("Flash block device program size: %llu\n", bd->get_program_size());
    printf("Flash block device erase size: %llu\n", bd->get_erase_size());
}

void print_byte(uint8_t *key, int size_of_key)
{
    for (int i = 0; i < size_of_key; i++) {
        printf("0x%X ", key[i]);
    }
    printf("\n");
}

int main()
{
    // Initialize STSafe-A110
    if (stsafea110.init()) {
        printf("Failed to init STSafe-A110");
    }
    flash_init();

    printf("---------------- Pairing Key ----------------\n");
    while (!pairing_status) {
        led1 = 0;
        if (stsafea110.pairing(Host_MAC_Cipher_Key, write_configuration) == 0) {
            led1 = 1;
            printf("Pairing succeeded \n\r");
            pairing_status = 1;
        } else {
            printf("Pairing Failed \n\r");
            led1 = !led1;
            ThisThread::sleep_for(HALF_PERIOD);
        }
    }
    printf("Read partition 0: %d\n", stsafea110.read_data_partition(0, read_key, sizeof(read_key)));
    printf("Is Key protected ? ");
    if (read_key != Host_MAC_Cipher_Key) {
        printf("True\n");
    } else {
        printf("False\n");
    }
    print_byte(read_key, sizeof(read_key));

    printf("-------- Write and Read on partition --------\n");
    // Write data
    uint8_t data[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
    printf("Write: %d\n", stsafea110.update_data_partition(2, data, sizeof(data)));
    // Read data
    uint8_t read_data[10];
    printf("Read: %d\n", stsafea110.read_data_partition(2, read_data, sizeof(read_data)));
    print_byte(read_data, sizeof(read_data));

    while (true) {
        ThisThread::sleep_for(HALF_PERIOD);
    }

    return 0;
}
