# ModelView3D

<img width="879" height="709" alt="Image" src="https://github.com/user-attachments/assets/a6a6910b-117b-4122-b997-51bc6cf1b097" />

# Build instructions

### Debug
cmake -S . -B .\build\debug -DCMAKE_BUILD_TYPE=Debug
cmake --build .\build\debug\ --config Debug
.\build\debug\bin\Debug\ModelView3D.exe

### Release
cmake -S . -B .\build\release -DCMAKE_BUILD_TYPE=Release
cmake --build .\build\release\ --config Release
.\build\release\bin\Release\ModelView3D.exe

