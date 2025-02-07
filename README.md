# 3D Game Engine

## 📌 Overview
This is a **simple 3D game engine** designed for showcasing **memory management, graphics programming**, and efficient system design. It is built using **C++17** and leverages modern libraries such as **GLFW, GLEW, SDL2, ImGui, Bullet Physics, and spdlog**.

## 🎯 Features
- **Core Engine**: Handles application lifecycle and event management.
- **Memory Management**: Efficient allocation and deallocation with `MemoryManager`.
- **Job System**: Multi-threaded task execution with `JobSystem`.
- **Renderer**: OpenGL-based rendering pipeline.
- **Physics**: Uses Bullet Physics for realistic object interactions.
- **File System**: Handles asset loading and file I/O operations.
- **Logging System**: Uses `spdlog` for structured logging.

---

## 🔧 Installation & Setup

### **1️⃣ Clone the Repository**

```sh
 git clone https://github.com/ismailab1/3DGameEngine.git
 cd 3DGameEngine
 '''

 This project uses vcpkg to manage dependencies. Ensure you have vcpkg installed, then run:

 ```sh
 vcpkg install glfw3 glew tbb sdl2 imgui nlohmann-json spdlog bullet3 assimp
 '''

 Generate Build Files with CMake

 ```sh
 cmake -B out/build
 '''

 Compile the Engine

 ```sh
 cmake --build out/build --config Debug
 '''

 🚀 Running the Engine

 ```sh
 cd out/build/Sandbox/Debug
./Sandbox.exe
'''

Alternatively, in Visual Studio:

Set Sandbox.exe as the Startup Project.

Press F5 to run in debug mode.

Running Tests
```sh
cd out/build/tests/Debug
./3DGameEngineTests.exe
'''

3DGameEngine/
│── engine/                 # Core Engine
│   ├── include/            # Header files
│   ├── src/                # Implementation
│   ├── CMakeLists.txt      # Engine build setup
│
│── Sandbox/                # Testing Environment
│   ├── Sandbox.cpp         # Main entry for debugging
│   ├── CMakeLists.txt      # Sandbox build setup
│
│── tests/                  # Unit Tests
│   ├── test_main.cpp       # Catch2 test entry
│   ├── CMakeLists.txt      # Test setup
│
│── CMakeLists.txt          # Root CMake setup
│── README.md               # This file

Dependencies

GLFW - Windowing & Input

GLEW - OpenGL Extension Wrangler

SDL2 - Alternative input and window handling

ImGui - GUI for debugging

Bullet Physics - Physics simulation

nlohmann-json - JSON parsing

spdlog - Logging

TBB - Multi-threading

Assimp - Asset importing

 License

This project is licensed under the MIT License.

Contact

For any inquiries, reach out at ismailabdullah1129@gmail.com# 3D Game Engine

## 📌 Overview
This is a **simple 3D game engine** designed for showcasing **memory management, graphics programming**, and efficient system design. It is built using **C++17** and leverages modern libraries such as **GLFW, GLEW, SDL2, ImGui, Bullet Physics, and spdlog**.

## 🎯 Features
- **Core Engine**: Handles application lifecycle and event management.
- **Memory Management**: Efficient allocation and deallocation with `MemoryManager`.
- **Job System**: Multi-threaded task execution with `JobSystem`.
- **Renderer**: OpenGL-based rendering pipeline.
- **Physics**: Uses Bullet Physics for realistic object interactions.
- **File System**: Handles asset loading and file I/O operations.
- **Logging System**: Uses `spdlog` for structured logging.

---

## 🔧 Installation & Setup

### 1️⃣ Clone the Repository
### 2️⃣ Install Dependencies
This project uses vcpkg to manage dependencies. Ensure you have vcpkg installed, then run:
### 3️⃣ Generate Build Files with CMake
### 4️⃣ Compile the Engine
### 🚀 Running the Engine
Alternatively, in Visual Studio:
- Set `Sandbox.exe` as the Startup Project.
- Press `F5` to run in debug mode.

### 🧪 Running Tests
## 📁 Project Structure
## 📦 Dependencies
- **GLFW** - Windowing & Input
- **GLEW** - OpenGL Extension Wrangler
- **SDL2** - Alternative input and window handling
- **ImGui** - GUI for debugging
- **Bullet Physics** - Physics simulation
- **nlohmann-json** - JSON parsing
- **spdlog** - Logging
- **TBB** - Multi-threading
- **Assimp** - Asset importing

## 📄 License
This project is licensed under the MIT License.

## 📧 Contact
For any inquiries, reach out at ismailabdullah1129@gmail.com
