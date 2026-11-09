# Kernel source/build directory
KDIR := $(HOME)/embedded-lab/linux/kernel-6.6

# Target architecture
ARCH := arm64

# Cross compiler
CROSS_COMPILE := aarch64-linux-gnu-

# External kernel module
obj-m += major_minor_module.o

# Build target
all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

# Clean generated files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean