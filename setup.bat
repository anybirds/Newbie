@echo off

cd c:/
if not exist vcpkg\ (
    git clone https://github.com/Microsoft/vcpkg.git
)
cd c:/vcpkg
bootstrap-vcpkg.bat -disableMetrics
vcpkg integrate install
vcpkg install glew:x64-windows
vcpkg install glfw3:x64-windows
vcpkg install glm:x64-windows
vcpkg install assimp:x64-windows
vcpkg install stb:x64-windows
vcpkg install zlib:x64-windows
vcpkg install nlohmann-json:x64-windows

cd c:/Newbie/Engine
cmake -G "Visual Studio 16 2019 " -A x64 -B "../build/Engine"
cd c:/Newbie/build/Engine
cmake --build . --config Release

cd c:/Newbie/Editor
cmake -G "Visual Studio 16 2019" -A x64 -B "../build/Editor"
cd c:/Newbie/build/Editor
cmake --build . --config Release

cd c:/Newbie/Samples/Lofi
cmake -G "Visual Studio 16 2019" -A x64 -B "../build/Samples/Lofi"
cd c:/Newbie/build/Samples/Lofi
cmake --build . --config Release
