# ModelView3D

<img width="879" height="709" alt="Image" src="https://github.com/user-attachments/assets/a6a6910b-117b-4122-b997-51bc6cf1b097" />

**ModelView3D** is a lightweight C++ application for viewing and interacting with 3D models. It supports camera controls, fixed and dynamic lighting, and real-time rendering using OpenGL.

## Technologies Used

- **Eigen** - A C++ template library for linear algebra, matrices, and vectors.
- **GLFW** - A cross-platform library for creating windows, contexts, and handling input.
- **GLAD** - OpenGL loader library that manages function pointers for modern OpenGL.
- **Libigl** - A lightweight C++ geometry processing library for 3D meshes.

## Requirements

This project uses [vcpkg](https://github.com/microsoft/vcpkg) to manage dependencies. Follow these steps:

1. **Clone and bootstrap vcpkg**
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat -disableMetrics
vcpkg integrate install
```

2. **Install required libraries (x64 Windows triplet)**
```bash
vcpkg install eigen3 --triplet x64-windows
vcpkg install glfw3 --triplet x64-windows
vcpkg install glad --triplet x64-windows
vcpkg install libigl --triplet x64-windows
```

**Adjust --triplet if you need a different architecture (e.g., x86)**

## Build instructions

### Debug
```bash
cmake -S . -B .\build\debug -DCMAKE_BUILD_TYPE=Debug
cmake --build .\build\debug\ --config Debug
.\build\debug\bin\Debug\ModelView3D.exe
```

### Release
```bash
cmake -S . -B .\build\release -DCMAKE_BUILD_TYPE=Release
cmake --build .\build\release\ --config Release
.\build\release\bin\Release\ModelView3D.exe
```
