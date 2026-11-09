# Dynamic Major and Minor Number Allocation – Linux Kernel Module

## Overview

This project demonstrates how to dynamically allocate a character device number range using the Linux kernel API `alloc_chrdev_region()`.

Unlike static device number registration, where the developer specifies the major and minor numbers, dynamic allocation allows the Linux kernel to select an available major number.

The module registers the allocated device number range and releases it when the module is unloaded.

## Learning Objectives

- Understand Linux character device major and minor numbers.
- Understand the difference between static and dynamic device number allocation.
- Learn to use `alloc_chrdev_region()`.
- Learn to release allocated device numbers using `unregister_chrdev_region()`.
- Verify registered character device numbers through `/proc/devices`.

## Environment

| Component | Configuration |
|---|---|
| Host OS | Ubuntu 22.04 |
| Target Architecture | ARM64 (AArch64) |
| Kernel Version | 6.6.0 |
| Emulator | QEMU |
| Cross Compiler | aarch64-linux-gnu-gcc |
| Root Filesystem | BusyBox Initramfs |

## Repository Structure

```text
embedded-lab/
│
├── linux/
│   └── drivers/
│       └── major_minor_dyn/
│           ├── dyn_maj_min.c
│           ├── Makefile
│           └── README.md
│
├── rootfs/
│   └── lib/
│       └── modules/
│           └── 6.6.0/
│               └── extra/
│                   └── dyn_maj_min.ko
│
└── scripts/
    ├── initramfs.sh
    └── boot_qemu.sh
```

## Source Code

File: `dyn_maj_min.c`

```c
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

dev_t dev = 0;

static int __init my_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev, 1, 1, "sys_dev");

    if (ret < 0)
    {
        pr_err("Cannot allocate character device region\n");
        return ret;
    }

    pr_info("major = %d minor = %d\n", MAJOR(dev), MINOR(dev));
    pr_info("module inserted\n");

    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev_region(dev, 1);
    pr_info("Removed the char device region\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("PUJITHA");
MODULE_DESCRIPTION("DYNAMIC MAJOR MINOR NUMBER ALLOCATION");
MODULE_LICENSE("GPL");
MODULE_VERSION("V0.1");
```

## Understanding the Kernel APIs

### 1. `alloc_chrdev_region()`

```c
alloc_chrdev_region(&dev, 1, 1, "sys_dev");
```

Function prototype:

```c
int alloc_chrdev_region(dev_t *dev,
                        unsigned baseminor,
                        unsigned count,
                        const char *name);
```

| Argument | Description |
|---|---|
| `&dev` | Pointer where the allocated device number is stored |
| `1` | Starting minor number |
| `1` | Number of device numbers to allocate |
| `"sys_dev"` | Device name displayed in `/proc/devices` |

The kernel dynamically selects an available major number and allocates the requested device number range.

In this project:

- Major number: Allocated dynamically by the kernel.
- Starting minor number: 1.
- Number of device numbers: 1.
- Device name: `sys_dev`.

### 2. `MAJOR()` and `MINOR()`

```c
MAJOR(dev);
MINOR(dev);
```

These macros extract the major and minor numbers from the `dev_t` variable.

### 3. `unregister_chrdev_region()`

```c
unregister_chrdev_region(dev, 1);
```

Releases the previously allocated device number range when the module is unloaded.

## Build Instructions

Navigate to the driver directory:

```bash
cd ~/embedded-lab/linux/drivers/major_minor_dyn
```

Build the kernel module:

```bash
make
```

Copy the generated module into the QEMU root filesystem:

```bash
cp dyn_maj_min.ko ~/embedded-lab/rootfs/lib/modules/6.6.0/extra/
```

Rebuild the initramfs:

```bash
cd ~/embedded-lab
./scripts/initramfs.sh
```

## QEMU Testing

Start the ARM64 QEMU environment:

```bash
./scripts/boot_qemu.sh
```

Inside the QEMU shell, navigate to the module directory:

```bash
cd /lib/modules/6.6.0/extra/
```

### 1. Check device registrations before loading

```bash
cat /proc/devices
```

The `sys_dev` entry should not be present before loading this module.

### 2. Load the module

```bash
insmod dyn_maj_min.ko
```

### 3. Check kernel messages

```bash
dmesg | tail
```

Observed output:

```text
dyn_maj_min: loading out-of-tree module taints kernel.
major = 511 minor = 1
module inserted
```

The kernel dynamically allocated:

- Major number: `511`
- Minor number: `1`

### 4. Verify registration

```bash
cat /proc/devices
```

Observed entry:

```text
Character devices:
...
254 gpiochip
511 sys_dev
```

This confirms that the character device number range was successfully registered.

### 5. Unload the module

```bash
rmmod dyn_maj_min
```

Observed kernel message:

```text
Removed the char device region
```

The allocated device number range is released.

## Static vs Dynamic Device Number Allocation

| Feature | Static Allocation | Dynamic Allocation |
|---|---|---|
| API | `register_chrdev_region()` | `alloc_chrdev_region()` |
| Major number | Specified by developer | Selected by kernel |
| Minor number | Specified by developer | Starting minor specified by developer |
| Use case | Known, fixed device numbers | Preferred when the major number need not be fixed |

## Important Note

This module only reserves a character device number range.

It does **not** yet:
- Register a `struct cdev`.
- Implement `file_operations`.
- Create a device node under `/dev`.
- Support application-level `open()`, `read()`, or `write()` operations.

These features will be implemented in the next stage of character device driver development.

## Learning Outcome

Successfully implemented and tested dynamic character device number allocation in an ARM64 QEMU environment.

Verified that:

1. `alloc_chrdev_region()` dynamically allocated major number `511`.
2. Minor number `1` was assigned as requested.
3. `/proc/devices` displayed the registered device name `sys_dev`.
4. `unregister_chrdev_region()` released the allocated range during module removal.

## Next Step

Implement a character device using:

- `struct cdev`
- `cdev_init()`
- `cdev_add()`
- `file_operations`
- `class_create()`
- `device_create()`

The next goal is to create a device node under `/dev` and implement basic file operations.