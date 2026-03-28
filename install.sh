#!/bin/bash

# Simple English Installer for Ultra HDR Viewer
echo "--- Checking System Dependencies ---"

if [ -f /etc/arch-release ] || [ -f /etc/cachyos-release ]; then
    echo "System: CachyOS / Arch Linux detected (Primary Support)"
    sudo pacman -S --needed --noconfirm base-devel cmake qt6-base qt6-imageformats
elif [ -f /etc/fedora-release ]; then
    echo "System: Fedora detected"
    sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qtimageformats
else
    echo "Unknown OS. Please install Qt6, CMake, and GCC manually."
    exit 1
fi

echo "--- Building Ultra HDR Viewer ---"
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)

echo "--- Installing ---"
sudo make install

echo "--- Success! Run 'UltraHDRViewer' to start ---"
