#!/bin/bash
# Set working directory to script location
cd "$(dirname "$0")"

echo "--- Starting Ultra HDR Viewer Installation ---"

# 1. Detect OS and install dependencies
if [ -f /etc/fedora-release ]; then
    echo "--- Fedora detected. Installing dependencies (Qt6, CMake, GCC)... ---"
    sudo dnf install qt6-qtbase-devel qt6-qtdeclarative-devel cmake gcc-c++ -y
elif [ -f /etc/arch-release ] || [ -d /etc/cachyos ]; then
    echo "--- Arch/CachyOS detected. Installing dependencies (Qt6, CMake, GCC)... ---"
    sudo pacman -S --needed qt6-base cmake gcc --noconfirm
fi

# 2. Cleanup old shortcuts
echo "--- Cleaning up legacy desktop entries... ---"
sudo rm -f /usr/share/applications/ultraviewer.desktop /usr/share/applications/ultrahdrviewer.desktop
rm -f ~/.local/share/applications/*UltraViewer* ~/.local/share/applications/*ultrahdrviewer*

# 3. Build process
echo "--- Building the project from source... ---"
sudo rm -rf build && mkdir build && cd build
cmake ..
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed! Please check your CMakeLists.txt."
    exit 1
fi

make -j$(nproc)
sudo make install
cd ..

# 4. Icon Installation
if [ -f "icon.png" ]; then
    echo "--- Installing application icon to system path... ---"
    sudo mkdir -p /usr/local/share/icons
    sudo cp icon.png /usr/local/share/icons/ultrahdrviewer.png
fi

# 5. Create Desktop Entry with %F for batch selection support
echo "--- Registering Ultra HDR Viewer in the system menu... ---"
sudo bash -c 'cat > /usr/share/applications/ultrahdrviewer.desktop <<EOF
[Desktop Entry]
Name=Ultra HDR Viewer
Exec=/usr/local/bin/UltraHDRViewer %F
Icon=/usr/local/share/icons/ultrahdrviewer.png
Type=Application
MimeType=image/jpeg;image/png;image/webp;image/jxl;
Categories=Graphics;
Terminal=false
StartupNotify=true
EOF'

# Update system desktop database
sudo update-desktop-database /usr/share/applications/

echo "--- INSTALLATION FINISHED SUCCESSFULLY! ---"
echo "You can now open HDR images using Ultra HDR Viewer from your file manager."
