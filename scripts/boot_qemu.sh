cd ~/embedded-lab/linux/kernel-6.6/arch/arm64/boot

qemu-system-aarch64 \
    -machine virt \
    -cpu cortex-a57 \
    -m 512M \
    -kernel Image \
    -initrd ~/embedded-lab/rootfs.cpio.gz \
    -append "console=ttyAMA0 rdinit=/init" \
    -nographic