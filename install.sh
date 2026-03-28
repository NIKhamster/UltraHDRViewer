#!/bin/bash

# Colors
GREEN='\033[0;32m'
NC='\033[0m'

echo -e "${GREEN}--- Checking Dependencies ---${NC}"
if [ -f /etc/arch-release ] || [ -f /etc/cachyos-release ]; then
    sudo pacman -S --needed --noconfirm base-devel cmake qt6-base qt6-imageformats
elif [ -f /etc/fedora-release ]; then
    sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qtimageformats qt6-qtwayland
fi

echo -e "${GREEN}--- Building Project ---${NC}"
rm -rf build && mkdir build && cd build
cmake .. && make -j$(nproc)
sudo make install

echo -e "${GREEN}--- System Integration ---${NC}"
sudo mkdir -p /usr/local/share/icons
sudo cp ../icon.png /usr/local/share/icons/ultrahdrviewer.png

sudo bash -c 'cat > /usr/share/applications/ultrahdrviewer.desktop <<EOF
[Desktop Entry]
Name=Ultra HDR Viewer
Exec=UltraHDRViewer %f
Icon=/usr/local/share/icons/ultrahdrviewer.png
Type=Application
Categories=Graphics;
MimeType=image/jpeg;image/png;image/webp;image/jxl;
Terminal=false
EOF'

sudo update-desktop-database /usr/share/applications/
echo -e "${GREEN}--- Done! Ready to use ---${NC}"
