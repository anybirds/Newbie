cd /
if [-d "vcpkg/"]; then
    git clone https://github.com/Microsoft/vcpkg.git
fi
cd /vcpkg
vcpkg integrate install
vcpkg install glew
vcpkg install glfw3
vcpkg install glm
vcpkg install assimp
vcpkg install stb
vcpkg install zlib
vcpkg install nlohmann-json

cd /Newbie/Engine
cmake -G "Visual Studio 16 2019 " -A x64 -B "../Build/Engine"
cd /Newbie/Build/Engine
cmake --build . --config Release
cd /Newbie/Editor
cmake -G "Visual Studio 16 2019" -A x64 -B "../Build/Editor"
cd /Newbie/Build/Editor
cmake --build . --config Release
cd /Newbie/App
cmake -G "Visual Studio 16 2019" -A x64 -B "../Build/App"
cd /Newbie/Build/App
cmake --build . --config Release
