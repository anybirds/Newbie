cd /
if [! -d "vcpkg/"]; then
    sudo git clone https://github.com/Microsoft/vcpkg.git
fi
cd /vcpkg
bootstrap-vcpkg.bat -disableMetrics
vcpkg integrate install
vcpkg install glew
vcpkg install glfw3
vcpkg install glm
vcpkg install assimp
vcpkg install stb
vcpkg install zlib
vcpkg install nlohmann-json

cd /Newbie/Engine
cmake -G "Visual Studio 16 2019 " -A x64 -B "../build/Engine"
cd /Newbie/build/Engine
cmake --build . --config Release

cd /Newbie/Editor
cmake -G "Visual Studio 16 2019" -A x64 -B "../build/Editor"
cd /Newbie/build/Editor
cmake --build . --config Release

cd c:/Newbie/Samples/Lofi
cmake -G "Visual Studio 16 2019" -A x64 -B "../build/Samples/Lofi"
cd c:/Newbie/build/Samples/Lofi
cmake --build . --config Release
