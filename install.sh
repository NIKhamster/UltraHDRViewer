#!/bin/bash
# Universal Installer with Mime-type registration

echo "--- Installing Dependencies ---"
if [ -f /etc/arch-release ] || [ -f /etc/cachyos-release ]; then
    sudo pacman -S --needed --noconfirm base-devel cmake qt6-base qt6-imageformats
elif [ -f /etc/fedora-release ]; then
    sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qtimageformats
fi

echo "--- Building ---"
rm -rf build && mkdir build && cd build
cmake .. && make -j$(nproc)
sudo make install

echo "--- Desktop Integration ---"
# Создаем иконку
sudo mkdir -p /usr/local/share/icons
sudo cp ../icon.png /usr/local/share/icons/ultrahdrviewer.png

# Создаем .desktop файл с поддержкой аргументов %f
sudo bash -c 'cat > /usr/share/applications/ultrahdrviewer.desktop <<EOF
[Desktop Entry]
Name=Ultra HDR Viewer
Exec=UltraHDRViewer %f
Icon=/usr/local/share/icons/ultrahdrviewer.png
Type=Application
Categories=Graphics;
MimeType=image/jpeg;image/png;image/webp;image/jxl;
Terminal=false
NoDisplay=false
EOF'

# Обновляем базу данных приложений (важно для Fedora!)
sudo update-desktop-database /usr/share/applications/

echo "--- Done! Check 'Open With' menu now ---"
