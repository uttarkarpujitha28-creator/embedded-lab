# Manual Device Node Creation

## Objective

Learn how Linux dynamically allocates a character-device number and how to manually create a device node using `mknod`.

## Concepts

### 1. Dynamic Major and Minor Allocation

The module uses:

```c
alloc_chrdev_region(&dev, 0, 1, "Embetronicx_Dev");
```

- `&dev`: Stores the allocated device number.
- `0`: Starting minor number.
- `1`: Number of device numbers to allocate.
- `"Embetronicx_Dev"`: Device name displayed in `/proc/devices`.

The kernel dynamically selects an available major number.

### 2. Check the Allocated Major Number

After inserting the module:

```bash
insmod manual.ko
```

Check:

```bash
cat /proc/devices
```

Example output:

```text
511 Embetronicx_Dev
```

Note: `/proc/devices` displays the major number and device name, not the minor number.

The module prints both numbers:

```text
Major num = 511 Minor num = 0
```

### 3. Create a Device Node Manually

Use:

```bash
mknod -m 666 /dev/manual_file c 511 0
```

Syntax:

```bash
mknod [options] <device_node> <type> <major> <minor>
```

- `/dev/manual_file`: Device-node name.
- `c`: Character device.
- `511`: Allocated major number.
- `0`: Allocated minor number.
- `-m 666`: Read/write permissions for all users.

Verify:

```bash
ls -l /dev/manual_file
```

Expected:

```text
crw-rw-rw- 1 root root 511, 0 ... /dev/manual_file
```

The `c` indicates a character device.

### 4. Important: What Does mknod Do?

`alloc_chrdev_region()` reserves a device number with the kernel.

`mknod` creates a special file in `/dev` that contains the specified major and minor numbers.

It does not allocate another device number or register a functional driver.

### 5. Removing the Module and Device Node

Remove the module:

```bash
rmmod manual
```

Remove the device node separately:

```bash
rm /dev/manual_file
```

Unloading the module releases the allocated device number, but does not automatically delete the manually created device node.

## Important Limitation

This module only reserves a character-device number.

It does not yet implement:

- `struct cdev`
- `cdev_init()`
- `cdev_add()`
- `struct file_operations`
- `open()`, `read()`, `write()` or `release()`

Therefore, the device node exists, but it is not yet connected to a functional character-device driver.

Attempting to access it using `cat` will fail because no character-device implementation has been registered.

## Testing Environment

- Architecture: ARM64
- Platform: QEMU virt
- Kernel version: 6.6.0
- Root filesystem: BusyBox initramfs

## Test Results

Successfully tested inside ARM64 QEMU:

```bash
insmod manual.ko
cat /proc/devices
mknod -m 666 /dev/manual_file c 511 0
ls -l /dev/manual_file
```

The kernel allocated major number `511` and minor number `0`.

The device node was successfully created with the matching numbers.

## Key Takeaways

1. `alloc_chrdev_region()` dynamically allocates a device number.
2. `MAJOR(dev)` extracts the major number.
3. `MINOR(dev)` extracts the minor number.
4. `/proc/devices` displays registered major numbers.
5. `mknod` creates a device node using the supplied major and minor numbers.
6. A device node alone does not make a functional driver.
7. The next step is registering a `cdev` and implementing file operations.