# mbed_stm-stsafe-A110-example
Mbed-OS example usage of STSafe-A110 from STM.

## Requirements
### Hardware requirements
The following boards are required:
- Zest_Security_SecureElement
- Any Zest Core

### Software requirements
mbed_stm-stsafe-A110-example makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):
- [STSafe-A110](https://github.com/catie-aq/mbed_stm-stsafe-A110)

## Usage
To clone **and** deploy the project in one command, use `mbed import` and skip to the
target enabling instructions:
```shell
mbed import https://github.com/catie-aq/mbed_stm-stsafe-a110-example.git mbed_stm-stsafe-a110-example
```

Alternatively:

- Clone to "mbed_stm-stsafe-a110-example" and enter it:
  ```shell
  git clone https://github.com/catie-aq/mbed_stm-stsafe-a110-example.git mbed_stm-stsafe-a110-example
  cd mbed_stm-stsafe-a110-example
  ```

- Deploy software requirements with:
  ```shell
  mbed deploy
  ```

- Clone custom target repository if necessary:
  ```shell
  git clone YOUR_CUSTOM_TARGET_REPOSITORY your-custom-target
  ```

Define your target and toolchain:
```shell
cp your-custom-target/custom_targets.json . # In case of custom target
mbed target {{cookiecutter.mbed_os_target}}
mbed toolchain {{cookiecutter.toolchain}}
```

Compile the project:
```shell
mbed compile
```

Program the target device with a Segger J-Link debug probe and
[`sixtron_flash`](https://github.com/catie-aq/6tron-flash) tool:
```shell
sixtron_flash YOUR_CUSTOM_TARGET BUILD/YOUR_CUSTOM_TARGET/GCC_ARM/mbed_stm-stsafe-a110-example.elf
```

Debug on the target device with the probe and Segger
[Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger)
software.
