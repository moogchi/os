#!/bin/bash
set -e

# Make sure the ISO is built
if [ ! -f build/myos.iso ]; then
    echo "Building OS first..."
    ./build.sh
fi

# Run in QEMU
echo "Starting Copium OS in QEMU..."
qemu-system-i386 -cdrom build/myos.iso
