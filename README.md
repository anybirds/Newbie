# Newbie Engine
Can one undergraduate student make a fully-functional game engine? Let's try answering to this question.

## Goal
* Study and research about how the game engine is built in general.
* Come up with abstractions such as Project, Scene, GameObject, Renderer, Collider and many more.
* Make a GUI editor that allows users to inspect and modify objects handily.
* Create a simple 3D game with this engine. 

## Feature

## Build
* Install [Visual Studio](https://visualstudio.microsoft.com/).
* Install [CMake](https://cmake.org/).
* Install [Git](https://git-scm.com/). 
* Install [vcpkg](https://github.com/icrosoft/vcpkg).
  * Open **Windows Command Prompt** with administrator privilege
  * Goto **C Drive** 
  * Type **git clone https://github.com/Microsoft/vcpkg.git**
  * Type **cd vcpkg**
  * Type **vcpkg integrate install**
  * Register the environment variable **VCPKG_DEFAULT_TRIPLET** to match the target, for example **x64-windows**
* Install packages GLFW3 / GLEW / GLM / Assimp / SOIL2.
  * Reopen **Windows Command Prompt** with administrator privilege
  * Type **vcpkg install glew**
  * Type **vcpkg install glfw3**
  * Type **vcpkg install glm**
  * Type **vcpkg install assimp**
  * Type **vcpkg install stb**
* Clone this project inside the directory you want to build.
* Generate build system from source files.
  * Open **CMake**
  * Type in the source path
  * Type in the build path
  * Press **Configure** button
  * Select **Visual Studio** as generator
  * Select the optional platform for generator to match the target
  * Press **Finish** button
  * Press **Build** button
* Run solution file
  * Open the solution file
  * Set **Application** as **Startup Project**
  * Build and Run

## Documentation
All descriptions are directly written to the code itself. [Github Wiki](https://github.com/anybirds/Newbie/wiki) is also available.

## Copyright
Copyright © 2021 Hajun Kim, Undergraduate student in KAIST. All rights reserved. 
