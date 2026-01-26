#!/bin/bash
set -e

# Build the Docker image if it doesn't exist
if [[ "$(docker images -q myos-builder 2> /dev/null)" == "" ]]; then
    echo "Building Docker image..."
    docker build -t myos-builder .
fi

# Build the OS
echo "Building Copium OS..."
docker run --rm -v $(pwd):/workspace -u $(id -u):$(id -g) myos-builder make all

echo "✓ Build complete! ISO at: build/myos.iso"
