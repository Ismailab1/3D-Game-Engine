# 3D Game Engine

A simple 3D game engine built with modern C++ to demonstrate skills in system programming, memory management, job systems, and rendering pipelines. This project is designed for portfolio purposes and showcases the use of various libraries.

---

## **Features**
- Cross-platform rendering using OpenGL.
- Asset management with Assimp.
- Mathematical computations powered by GLM.
- Multi-threading using Intel TBB.
- Unit testing with Catch2.
- Built using CMake and vcpkg for dependency management.

---

## **Prerequisites**
Before setting up the project, ensure you have the following installed:

1. **Visual Studio 2022** (Community Edition or higher) with the following components:
   - Desktop Development with C++
   - CMake tools for Windows
   - MSVC Compiler Toolchain (latest version)

2. **CMake** (minimum version 3.20, included with Visual Studio).

3. **vcpkg** for dependency management.

   To install vcpkg:
   ```powershell
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```

4. Set the `VCPKG_ROOT` environment variable:
   ```powershell
   $env:VCPKG_ROOT = "path\to\vcpkg"
   ```

---

## **Dependencies**
The following libraries are managed by `vcpkg`:
- **GLFW**: Window and input management.
- **GLEW**: OpenGL Extension Wrangler.
- **GLM**: Mathematics library for graphics programming.
- **Assimp**: Asset Import Library for 3D model formats.
- **TBB**: Intel Threading Building Blocks for parallelism.
- **Catch2**: Unit testing framework.
- **SDL2**: Robust input handling for game controllers and extended devices.
- **Bullet Physics**: High-performance physics simulation and collision detection.
- **ImGui**: Debugging overlays and runtime GUI.
- **nlohmann-json**: JSON library for scene serialization and deserialization.
- **spdlog**: Fast logging library for debugging and runtime information.

To install these dependencies via `vcpkg`:
```powershell
vcpkg install glfw3:x64-windows glew:x64-windows glm:x64-windows assimp:x64-windows tbb:x64-windows catch2:x64-windows
vcpkg install sdl2:x64-windows sdl2-image:x64-windows
vcpkg install bullet3:x64-windows
vcpkg install imgui:x64-windows
vcpkg install nlohmann-json:x64-windows
vcpkg install spdlog:x64-windows
```

---

## **Project Setup**
### Clone the Repository
```powershell
git clone https://github.com/yourusername/3D-Game-Engine.git
cd 3D-Game-Engine
```

### Generate Build Files
Use CMake to generate the project files:
```powershell
cmake -S . -B .\out\build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" -DCMAKE_BUILD_TYPE=Debug
```

### Build the Project
To build the project, run:
```powershell
cmake --build .\out\build --config Debug
```

---

## **Run the Application**
After building, the executables will be located in:
```
out\build\Debug\
```

Run the main executable:
```powershell
.\out\build\Debug\3DGameEngine.exe
```

Run the unit tests:
```powershell
.\out\build\Debug\3DGameEngineTests.exe
```

---

## **Folder Structure**
```
3D-Game-Engine/
├── assets/             # Asset files for the engine (models, shaders, textures).
│   ├── models/         # 3D model files.
│   ├── shaders/        # Shader files for rendering.
│   └── textures/       # Texture files for materials.
├── cmake/              # CMake configuration files for build system customization.
├── docs/               # Documentation for the project.
├── external/           # Third-party libraries and dependencies.
├── include/            # Header files for the engine and game.
│   ├── engine/         # Engine-specific headers.
│   └── game/           # Game-specific headers.
├── src/                # Source files for the engine and game.
│   ├── engine/         # Engine-specific source files.
│   └── game/           # Game-specific source files.
├── tests/              # Unit tests for the engine and game.
│   ├── engine/         # Engine-specific test cases.
│   └── game/           # Game-specific test cases.
├── out/                # Build output (ignored in version control).
│   ├── Debug/          # Debug build output.
│   └── Release/        # Release build output.
├── .clang-format       # Code formatting rules for consistency.
├── .gitignore          # Git ignore file for excluded files and folders.
├── CMakeLists.txt      # Root CMake configuration file.
└── README.md           # Project setup instructions and details.
```

---

## **Troubleshooting**
1. **Error: Cannot find `vcpkg` dependencies**  
   Ensure the `VCPKG_ROOT` environment variable is set and the toolchain file is passed to CMake:
   ```powershell
   set VCPKG_ROOT=path\to\vcpkg
   cmake -S . -B .\out\build -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
   ```

2. **Error: Missing `main()`**  
   Ensure `src/game/Game.cpp` defines the `main()` function.

3. **Error: Cannot open library files like `assimp.lib`**  
   Verify the libraries are installed with vcpkg:
   ```powershell
   vcpkg list
   ```

---

## **License**
This project is open-source and licensed under the [AGPL-3.0 license](LICENSE).

---
