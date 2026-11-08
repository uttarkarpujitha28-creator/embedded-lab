# First ARM64 Linux Kernel Module

![Linux](https://img.shields.io/badge/Linux-Kernel%206.6.0-blue)
![Architecture](https://img.shields.io/badge/Architecture-ARM64-green)
![Language](https://img.shields.io/badge/Language-C-orange)
![Platform](https://img.shields.io/badge/Platform-QEMU-lightgrey)

## Overview

This project demonstrates the development, cross-compilation, loading, and unloading of a basic Linux kernel module.

The module was developed on an Ubuntu host and cross-compiled for ARM64. It was then loaded and tested inside an ARM64 Linux environment running on QEMU.

The objective of this exercise was to understand the Linux kernel module lifecycle and the interaction between user space and kernel space.

## Development Environment

| Component | Configuration |
|---|---|
| Host OS | Ubuntu 22.04 |
| Target Architecture | ARM64 (AArch64) |
| Linux Kernel | 6.6.0 |
| Compiler | AArch64 GNU GCC 11.4.0 |
| Emulator | QEMU |
| Programming Language | C |
| Module Build System | Kbuild |

## Project Structure

```text
hello_module/
├── hello_module.c
├── Makefile
├── hello_module.ko
└── README.md
```

## Kernel Module Implementation

The module uses the Linux kernel's module initialization and exit mechanisms.

### Initialization Function

The `hello_driver_init()` function is invoked when the module is loaded.

```c
static int __init hello_driver_init(void)
{
    pr_info("inside the hello code of the module\n");
    return 0;
}
```

### Exit Function

The `hello_driver_exit()` function is invoked when the module is unloaded.

```c
static void __exit hello_driver_exit(void)
{
    pr_info("at the exit\n");
}
```

### Module Entry and Exit Registration

```c
module_init(hello_driver_init);
module_exit(hello_driver_exit);
```

- `module_init()` registers the initialization function.
- `module_exit()` registers the cleanup function.
- `pr_info()` prints informational messages to the kernel log.

## Cross-Compilation

The module was cross-compiled for ARM64 using the kernel build system.

```bash
make
```

The Makefile uses:

```makefile
KDIR := $(HOME)/embedded-lab/linux/kernel-6.6
ARCH := arm64
CROSS_COMPILE := aarch64-linux-gnu-

obj-m += hello_module.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```

The generated `hello_module.ko` file is the loadable kernel module.

## QEMU Testing

The ARM64 kernel was booted using QEMU with a custom initramfs.

The compiled module was placed inside:

```text
/lib/modules/6.6.0/extra/hello_module.ko
```

### 1. Load the module

```bash
insmod hello_module.ko
```

Kernel log:

```text
hello_module: loading out-of-tree module taints kernel.
inside the hello code of the module
```

### 2. Verify the loaded module

```bash
lsmod
```

Observed output:

```text
hello_module 12288 0 - Live 0xffffb34e4d649000 (O)
```

This confirms that the module was successfully loaded into the running kernel.

### 3. Unload the module

```bash
rmmod hello_module
```

Kernel log:

```text
at the exit
```

This confirms that the module's exit function was executed successfully.

## Results

| Test | Result |
|---|---|
| ARM64 cross-compilation | PASS |
| QEMU Linux kernel boot | PASS |
| Module loading (`insmod`) | PASS |
| Module verification (`lsmod`) | PASS |
| Module unloading (`rmmod`) | PASS |
| Kernel log verification (`dmesg`) | PASS |

## Key Learnings

Through this exercise, I gained practical experience with:

- Linux kernel module architecture.
- Kernel module initialization and cleanup.
- `module_init()` and `module_exit()`.
- Kernel logging using `pr_info()`.
- ARM64 cross-compilation.
- Kbuild and external kernel modules.
- Loading and unloading modules dynamically.
- Testing kernel modules using QEMU.

## Next Steps

- Implement a Linux character device driver.
- Understand major and minor device numbers.
- Implement `file_operations`.
- Explore `open()`, `read()`, `write()`, and `release()`.
- Create a user-space application to communicate with the kernel driver.

---

**Note:** This is a basic loadable kernel module used to understand the Linux kernel module lifecycle. It is not yet a hardware-specific device driver.