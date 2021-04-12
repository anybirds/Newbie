@echo off

if not exist vcpkg\ (
    git clone https://github.com/Microsoft/vcpkg.git
)
cd vcpkg
call bootstrap-vcpkg.bat -disableMetrics
vcpkg integrate install
vcpkg install glew:x64-windows
vcpkg install glfw3:x64-windows
vcpkg install glm:x64-windows
vcpkg install assimp:x64-windows
vcpkg install stb:x64-windows
vcpkg install zlib:x64-windows
vcpkg install nlohmann-json:x64-windows

cd ..
cmake -G "Visual Studio 16 2019" -A x64 -B build
cmake --build build --config Release