FROM ubuntu:24.04

# Avoid interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN dpkg --add-architecture i386 && \
    apt-get update && apt-get install -y \
    build-essential \
    bison \
    flex \
    libgmp3-dev \
    libmpc-dev \
    libmpfr-dev \
    texinfo \
    wget \
    curl \
    unzip \
    grub-pc-bin \
    grub-common \
    xorriso \
    qemu-system-x86 \
    libc6:i386 \
    libstdc++6:i386 \
    && rm -rf /var/lib/apt/lists/*

# Download and install prebuilt i686-elf toolchain
RUN cd /tmp && \
    wget -q https://github.com/lordmilko/i686-elf-tools/releases/download/7.1.0/i686-elf-tools-linux.zip && \
    unzip -q i686-elf-tools-linux.zip -d /opt/cross && \
    chmod -R +x /opt/cross/bin/* && \
    rm -rf /tmp/*

# Add cross-compiler to PATH
ENV PATH="/opt/cross/bin:${PATH}"

# Set working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]