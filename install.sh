#!/bin/bash

echo "--- Определение системы ---"
if [ -f /etc/fedora-release ]; then
    echo "Система: Fedora"
    sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qtimageformats
elif [ -f /etc/arch-release ] || [ -f /etc/cachyos-release ]; then
    echo "Система: CachyOS / Arch"
    sudo pacman -S --needed --noconfirm base-devel cmake qt6-base qt6-imageformats
else
    echo "ОС не поддерживается этим скриптом. Установите зависимости вручную."
    exit 1
fi

echo "--- Очистка и сборка ---"
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)

echo "--- Установка ---"
sudo make install

echo "--- Готово! Ultra HDR Viewer установлен ---"
