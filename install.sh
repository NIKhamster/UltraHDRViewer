#!/bin/bash

# Define colors for better visibility
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}--- System Identification ---${NC}"

if [ -f /etc/fedora-release ]; then
    echo "OS: Fedora Detected"
    sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qtimageformats
elif [ -f /etc/arch-release ] || [ -f /etc/cachyos-release ]; then
    echo "OS: CachyOS / Arch Detected"
    sudo pacman -S --needed --noconfirm base-devel cmake qt6-base qt6-imageformats
else
    echo -e "${RED}Error: Operating system not supported by this script.${NC}"
    echo "Please install dependencies (Qt6, CMake, GCC) manually."
    exit 1
fi

echo -e "${GREEN}--- Cleaning and Building Project ---${NC}"

# Remove old build artifacts to ensure a fresh start
rm -rf build
mkdir build && cd build

# Configure and Compile
cmake ..
make -j$(nproc)

echo -e "${GREEN}--- Installation ---${NC}"

# Installs the binary to /usr/local/bin/
sudo make install

echo -e "${GREEN}--- Success! Ultra HDR Viewer has been installed ---${NC}"
echo "You can now run it from your terminal or 'Open With' menu."
