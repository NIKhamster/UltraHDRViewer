# 📸 Ultra HDR Viewer

<p align="center">
  <img src="icon.png" width="128" height="128" alt="UltraViewer Icon">
  <br>
  <b>Professional HDR Image Viewer for Linux (Fedora & CachyOS/Arch)</b>
</p>

---

## 📖 About the Project
**Ultra HDR Viewer** is a lightweight C++/Qt6 application designed for correct and vibrant display of HDR content. It features full 10-bit color support via hardware acceleration, smooth fade animations, and a focus on performance and minimalist design. No unnecessary borders or UI elements—just your content.

### ✨ Key Features:
* 🌈 **True HDR**: Native 10-bit color depth support via Qt6 Surface Format.
* 🎬 **Smooth Fade**: Aesthetically pleasing fade effect (650ms) when switching images.
* ⏱️ **Smart Slideshow**: Automatic viewing mode with a customizable delay (default 4.5s).
* 🛡️ **Clean Interface**: Borderless and focus-oriented design.

---

## ⌨️ Controls (Hotkeys)

| Key | Action |
| :--- | :--- |
| **Space (Пробел)** | **Slideshow**: Start / Stop (4.5s delay before skip) |
| **Arrow Right (→)** | Next image |
| **Arrow Left (←)** | Previous image |
| **Mouse Wheel** | Smooth Zoom (scaling) |
| **Esc** | Exit the application |

---

## 🚀 Easy Installation (Universal Guide)

The automated installers handle all dependencies (Qt6, CMake, GCC), clean the build directory, and compile the project for your system.

### 1. Prerequisite
Ensure all project files are in one folder: `main.cpp`, `viewer.h`, `CMakeLists.txt`, `icon.png`, and the installation scripts.

### 2. Run the Installer
Open a terminal in the project folder and run the command for your operating system:

#### **For Fedora:**
```bash
chmod +x install_fedora.sh && ./install_fedora.sh
