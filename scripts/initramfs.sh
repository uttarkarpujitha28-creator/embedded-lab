#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

echo "Building initramfs..."

(
    cd "$PROJECT_DIR/rootfs" &&
    find . -print0 | cpio --null -ov --format=newc
) | gzip -1 > "$PROJECT_DIR/rootfs.cpio.gz"

echo "Initramfs created at:"
echo "$PROJECT_DIR/rootfs.cpio.gz"