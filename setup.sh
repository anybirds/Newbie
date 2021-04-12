if [! -d "vcpkg/"]; then
    sudo git clone https://github.com/Microsoft/vcpkg.git
fi
cd vcpkg
sh bootstrap-vcpkg.sh -disableMetrics
vcpkg integrate install
vcpkg install glew
vcpkg install glfw3
vcpkg install glm
vcpkg install assimp
vcpkg install stb
vcpkg install zlib
vcpkg install nlohmann-json

cd ..
cmake -G "Unix Makefiles" -B build
cmake --build build