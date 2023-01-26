# mbed_stm-stsafe-A110-example
Mbed-OS example usage of STSafe-A110 from STM.

## Requirements
### Hardware requirements
The following boards are required:
- *List mbed_stm-stsafe-A110-example hardware requirements here*

### Software requirements
mbed_stm-stsafe-A110-example makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):
- *List mbed_stm-stsafe-A110-example software requirements here*

## Usage
To clone **and** deploy the project in one command, use `mbed import` and skip to the
target enabling instructions:
```shell
mbed import https://gitlab.com/catie_6tron/mbed_stm-stsafe-a110-example.git mbed_stm-stsafe-a110-example
```

Alternatively:

- Clone to "mbed_stm-stsafe-a110-example" and enter it:
  ```shell
  git clone https://gitlab.com/catie_6tron/mbed_stm-stsafe-a110-example.git mbed_stm-stsafe-a110-example
  cd mbed_stm-stsafe-a110-example
  ```

- Deploy software requirements with:
  ```shell
  mbed deploy
  ```

Enable the custom target:
```shell
cp zest-core-stm32h743zg/custom_targets.json .
```

Compile the project:
```shell
mbed compile
```

Program the target device with a Segger J-Link debug probe and
[`sixtron_flash`](https://github.com/catie-aq/6tron-flash) tool:
```shell
sixtron_flash stm32h743zg BUILD/ZEST_CORE_STM32H743ZG/GCC_ARM/mbed_stm-stsafe-a110-example.elf
```

Debug on the target device with the probe and Segger
[Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger)
software.
