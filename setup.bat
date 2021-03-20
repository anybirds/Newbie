@echo off

cd c:/
if exist vcpkg\ (
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
cmake -G "Visual Studio 16 2019 " -A x64 -B "../Build/Engine"
cd c:/Newbie/Build/Engine
cmake --build . --config Release
cd c:/Newbie/Editor
cmake -G "Visual Studio 16 2019" -A x64 -B "../Build/Editor"
cd c:/Newbie/Build/Editor
cmake --build . --config Release
cd c:/Newbie/App
cmake -G "Visual Studio 16 2019" -A x64 -B "../Build/App"
cd c:/Newbie/Build/App
cmake --build . --config Release
