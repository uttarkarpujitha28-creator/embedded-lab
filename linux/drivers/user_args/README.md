# Linux Kernel Module – User Arguments

![Linux](https://img.shields.io/badge/Linux-Kernel%206.6.0-black?logo=linux)
![Architecture](https://img.shields.io/badge/Architecture-ARM64-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Platform](https://img.shields.io/badge/Platform-QEMU-green)

## 1. Overview

This project demonstrates how to pass parameters to a Linux loadable kernel module and access or modify them at runtime.

The module implements three different kernel module parameter mechanisms:

- `module_param()` – Passing individual parameters.
- `module_param_array()` – Passing an array of values.
- `module_param_cb()` – Implementing custom parameter getter and setter callbacks.

It also demonstrates how to access module parameters through the Linux sysfs filesystem.

The module was cross-compiled for ARM64 and tested using an ARM64 Linux 6.6.0 kernel running on QEMU.

## 2. Development Environment

| Component | Configuration |
|---|---|
| Host OS | Ubuntu 22.04 |
| Target Architecture | ARM64 (AArch64) |
| Kernel Version | 6.6.0 |
| Compiler | AArch64 GCC |
| Emulator | QEMU |
| Root Filesystem | BusyBox-based initramfs |
| Programming Language | C |

## 3. Project Structure

```text
user_args/
│
├── user_args.c
├── Makefile
├── user_args.ko
└── README.md
```

## 4. Concepts Demonstrated

### 4.1 module_param()

Used to pass individual parameters to a kernel module during insertion.

Example:

```c
module_param(value, int, S_IWUSR | S_IRUSR);
module_param(name, charp, S_IWUSR | S_IRUSR);
```

Parameters:

- `value` – Integer parameter.
- `name` – Character pointer parameter.

### 4.2 module_param_array()

Used to pass multiple values through an array parameter.

Example:

```c
module_param_array(arr_val, int, NULL, S_IWUSR | S_IRUSR);
```

The array accepts multiple integer values separated by commas.

Example:

```bash
arr_val=1,2,3,4
```

### 4.3 module_param_cb()

Used when custom getter and setter operations are required for a module parameter.

Example:

```c
static const struct kernel_param_ops my_kp_ops =
{
    .get = param_get_int,
    .set = notify_cb,
};

module_param_cb(cb_val, &my_kp_ops, &cb_val, S_IWUSR | S_IRUSR);
```

The custom setter function:

```c
int notify_cb(const char *val, const struct kernel_param *kp)
{
    int res = param_set_int(val, kp);

    if (res == 0)
    {
        pr_info("callback notified\n");
        pr_info("value of the cb update is cb = %d\n", cb_val);
    }

    return res;
}
```

**Working:**

1. The setter receives the new parameter value.
2. `param_set_int()` converts the input string into an integer and updates the parameter.
3. If the update succeeds, the callback prints the updated value using `pr_info()`.

The callback is invoked when the parameter is initially set during module insertion and whenever it is subsequently modified through sysfs.

## 5. Building the Kernel Module

Navigate to the driver directory:

```bash
cd ~/embedded-lab/linux/drivers/user_args
```

Compile the module using:

```bash
make
```

The generated kernel module is:

```text
user_args.ko
```

## 6. Testing in QEMU

### Step 1: Boot ARM64 Linux

Start the QEMU environment:

```bash
cd ~/embedded-lab
./scripts/boot_qemu.sh
```

### Step 2: Verify the module

Inside the QEMU shell:

```bash
cd /lib/modules/6.6.0/extra/
ls
```

Expected output:

```text
hello_module.ko  user_args.ko
```

### Step 3: Insert the module with parameters

Execute:

```bash
insmod user_args.ko value=10 name=pujitha cb_val=20 arr_val=1,2,3,4
```

The following parameters are passed:

| Parameter | Value | Type |
|---|---|---|
| `value` | 10 | Integer |
| `name` | pujitha | Character pointer |
| `cb_val` | 20 | Integer with custom callback |
| `arr_val` | 1,2,3,4 | Integer array |

### Step 4: Verify kernel logs

Execute:

```bash
dmesg | tail -10
```

**Actual QEMU output:**

```text
[   88.923610] user_args: loading out-of-tree module taints kernel.
[   88.932585] callback notified
[   88.933169] value of the cb update is cb =20
[   88.937168] value = 10
[   88.937454] name = pujitha
[   88.937695] cb_val = 20
[   88.937887] arr_val=1
[   88.938070] arr_val=2
[   88.938234] arr_val=3
[   88.938364] arr_val=4
```

**Observation:**

- The module was successfully inserted.
- Individual parameters were received correctly.
- The integer array was populated with four values.
- The custom callback was invoked when `cb_val` was initialized to 20.

### Step 5: Modify the parameter through sysfs

Linux exposes module parameters through the sysfs filesystem.

The parameter is available at:

```text
/sys/module/user_args/parameters/cb_val
```

Update the value at runtime:

```bash
echo 72 > /sys/module/user_args/parameters/cb_val
```

Check the kernel logs:

```bash
dmesg | tail -10
```

**Actual output:**

```text
[  160.034619] callback notified
[  160.034884] value of the cb update is cb =72
```

The custom setter function is invoked automatically when the parameter is modified.

### Step 6: Read the updated parameter

Execute:

```bash
cat /sys/module/user_args/parameters/cb_val
```

Output:

```text
72
```

This confirms that the parameter was successfully updated through sysfs.

## 7. Test Results

| Test Case | Expected Result | Status |
|---|---|---|
| Module insertion | Module loads successfully | PASS |
| Integer parameter | Receives value 10 | PASS |
| String parameter | Receives value pujitha | PASS |
| Array parameter | Receives values 1, 2, 3, 4 | PASS |
| Custom callback | Invoked during initialization | PASS |
| Runtime parameter update | Updates cb_val to 72 | PASS |
| Sysfs parameter read | Returns updated value 72 | PASS |

## 8. Key Learnings

Through this exercise, the following Linux kernel concepts were explored:

- Loadable Kernel Modules (LKM).
- Passing parameters during module insertion.
- Integer and character pointer module parameters.
- Array parameters using `module_param_array()`.
- Custom parameter operations using `module_param_cb()`.
- Kernel logging using `pr_info()`.
- Runtime parameter modification through sysfs.
- Reading kernel module parameters from user space.
- ARM64 cross-compilation and QEMU-based testing.

## 9. Next Steps

The next stage of development will focus on Linux character device drivers, including:

- Device number allocation.
- `file_operations` structure.
- Implementing `open()`, `read()`, `write()` and `release()`.
- User-space and kernel-space communication.
- Device node creation under `/dev`.

---

**Project:** ARM64 Linux Kernel Module Development  
**Environment:** QEMU + Linux Kernel 6.6.0  
**Author:** Pujitha Uttharkar