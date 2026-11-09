# Linux Character Device – Major and Minor Number Registration

![Linux](https://img.shields.io/badge/Linux-Kernel%206.6.0-black?logo=linux)
![Architecture](https://img.shields.io/badge/Architecture-ARM64-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Platform](https://img.shields.io/badge/Platform-QEMU-green)

## 1. Overview

This project demonstrates how to register and unregister character device numbers in the Linux kernel.

Linux identifies character devices using two numbers:

- **Major Number:** Identifies the device driver associated with a device.
- **Minor Number:** Identifies a particular device handled by that driver.

The module uses the following kernel APIs:

- `MKDEV()` – Creates a device number from major and minor numbers.
- `MAJOR()` – Extracts the major number.
- `MINOR()` – Extracts the minor number.
- `register_chrdev_region()` – Reserves a specified device number range.
- `unregister_chrdev_region()` – Releases the reserved device number range.

The module was cross-compiled for ARM64 and tested using an ARM64 Linux 6.6.0 kernel running on QEMU.

## 2. Development Environment

| Component | Configuration |
|---|---|
| Host OS | Ubuntu 22.04 |
| Target Architecture | ARM64 / AArch64 |
| Kernel Version | Linux 6.6.0 |
| Compiler | AArch64 GCC |
| Emulator | QEMU |
| CPU | Cortex-A57 |
| Root Filesystem | BusyBox-based initramfs |
| Programming Language | C |

## 3. Project Structure

```text
major_minor_module/
│
├── major_minor_module.c
├── Makefile
├── major_minor_module.ko
└── README.md
```

## 4. Source Code

```c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>

dev_t dev = MKDEV(256, 0);

static int __init my_init(void)
{
    register_chrdev_region(dev, 1, "sample_num");

    pr_info("major num =%d minor num =%d\n",
            MAJOR(dev), MINOR(dev));

    pr_info("module inserted\n");

    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev_region(dev, 1);

    pr_info("unregistered the dev\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("PUJITHA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MAJOR AND MINOR NUMBER");
MODULE_VERSION("V0.1");
```

## 5. Concepts Demonstrated

### 5.1 Device Number (`dev_t`)

Linux uses the `dev_t` data type to represent a device number.

```c
dev_t dev = MKDEV(256, 0);
```

Here:

- Major number = 256
- Minor number = 0

The `MKDEV()` macro combines the major and minor numbers into a single device number.

### 5.2 MAJOR() and MINOR()

These macros extract the individual components from a device number.

```c
MAJOR(dev);
MINOR(dev);
```

For this project, the output is:

```text
major num =256 minor num =0
```

### 5.3 register_chrdev_region()

This function reserves a specified range of character device numbers.

```c
register_chrdev_region(dev, 1, "sample_num");
```

Arguments:

| Argument | Description |
|---|---|
| `dev` | Starting device number |
| `1` | Number of device numbers to reserve |
| `"sample_num"` | Device name displayed in `/proc/devices` |

The function reserves one device number with major number 256 and minor number 0.

**Important:** This function reserves the device number range. It does not create a character device, register file operations, or create a `/dev` node.

### 5.4 unregister_chrdev_region()

This function releases the previously reserved device number range.

```c
unregister_chrdev_region(dev, 1);
```

It should be called when the module is unloaded.

## 6. Building the Kernel Module

Navigate to the project directory:

```bash
cd ~/embedded-lab/linux/drivers/major_minor_module
```

Compile the module:

```bash
make
```

The generated kernel module is:

```text
major_minor_module.ko
```

## 7. Testing in QEMU

### Step 1: Boot ARM64 Linux

```bash
cd ~/embedded-lab
./scripts/boot_qemu.sh
```

### Step 2: Navigate to the module directory

Inside the QEMU shell:

```bash
cd /lib/modules/6.6.0/extra/
ls
```

Expected output:

```text
hello_module.ko
major_minor_module.o
major_minor_module.ko
user_args.ko
```

### Step 3: Insert the module

```bash
insmod major_minor_module.ko
```

Actual QEMU output:

```text
[   21.499437] major_minor_module: loading out-of-tree module taints kernel.
[   21.513358] major num =256 minor num =0
[   21.513645] module inserted
```

This confirms that the module was inserted and the major and minor numbers were extracted correctly.

### Step 4: Verify registration through /proc/devices

Execute:

```bash
cat /proc/devices
```

Relevant output:

```text
Character devices:
  1 mem
  2 pty
  3 ttyp
  4 /dev/vc/0
  4 tty
  5 /dev/tty
  ...
256 sample_num

Block devices:
  7 loop
  8 sd
  ...
```

**Observation:**

The entry:

```text
256 sample_num
```

confirms that the kernel has registered the character device number range under the name `sample_num`.

### Step 5: Unload the module

Execute:

```bash
rmmod major_minor_module
```

Actual QEMU output:

```text
[   57.252560] unregistered the dev
```

This confirms that the module's exit function was executed and the reserved device number range was released.

## 8. Test Results

| Test Case | Expected Result | Status |
|---|---|---|
| Module insertion | Module loads successfully | PASS |
| Device number creation | Major = 256, Minor = 0 | PASS |
| Device number registration | Entry appears in `/proc/devices` | PASS |
| Device name registration | `sample_num` appears | PASS |
| Module removal | Exit function executes | PASS |
| Device number unregistration | Reserved range released | PASS |

## 9. Key Learnings

Through this exercise, the following Linux kernel concepts were explored:

- Linux character device identification.
- `dev_t` data type.
- Major and minor device numbers.
- `MKDEV()`, `MAJOR()` and `MINOR()` macros.
- Static device number allocation.
- `register_chrdev_region()` API.
- `unregister_chrdev_region()` API.
- Device registration verification using `/proc/devices`.
- Kernel module initialization and cleanup.

## 10. Important Note and Future Improvements

This exercise focuses only on reserving and releasing character device numbers.

It does not yet implement actual character device functionality.

The next development stage will extend this module by introducing:

- `struct cdev`
- `cdev_init()`
- `cdev_add()`
- `file_operations`
- `open()`, `read()`, `write()` and `release()`
- `class_create()` and `device_create()`
- Device node creation under `/dev`

The module will then be extended to support communication between user-space applications and kernel-space code.

---

**Project:** Linux Character Device Number Registration  
**Environment:** ARM64 QEMU + Linux Kernel 6.6.0  
**Author:** Pujitha Uttharkar