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

cd /Newbie
cmake -G "Visual Studio 16 2019" -A x64 -B build
cmake --build build --config Release