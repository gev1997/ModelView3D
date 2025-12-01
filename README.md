# ModelView3D

# Build instructions
cmake -S . -B .\build\debug -DCMAKE_BUILD_TYPE=Debug
cmake --build .\build\debug\ --config Debug
.\build\debug\bin\Debug\ModelView3D.exe

cmake -S . -B .\build\release -DCMAKE_BUILD_TYPE=Release
cmake --build .\build\release\ --config Release
.\build\release\bin\Release\ModelView3D.exe
