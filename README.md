# Newbie Engine
Bare bone newbie-created game engine built upon OpenGL.

## Goal
* Study materials discussed in [LearnOpenGL](https://learnopengl.com/).
* Come up with abstractions(Object, Component, Renderer, Rigidbody, ...) that will help simplifying the implementation.
* Mimic some of the features provided in commodity game engines like Unity or Unreal.
* Provide an editor(GUI) that works in the front side of the engine.
* Create a simple 3D game by using this engine. 

## Feature

## Build
* Install [Visual Studio 2017](https://visualstudio.microsoft.com/) Community version.
* Install [CMake](https://cmake.org/) latest version.
* Install [Git](https://git-scm.com/) Windows 64-bit version. 
* Install [vcpkg](https://github.com/Microsoft/vcpkg).
  * Open **Windows PowerShell**
  * Goto **C Drive** 
  * Type **git clone https://github.com/Microsoft/vcpkg.git**
  * Type **cd .\vcpkg**
  * Type **.\vcpkg.exe integrate install**
  * Register the environment variable **VCPKG_DEFAULT_TRIPLET** to be **x64-windows**
* Install packages GLFW3 / GLEW / GLM / Assimp / SOIL.
  * Type **.\vcpkg.exe install glew**
  * Type **.\vcpkg.exe install glfw3**
  * Type **.\vcpkg.exe install glm**
  * Type **.\vcpkg.exe install assimp:x64-windows**
  * Type **.\vcpkg.exe install soil:x64-windows**
* Clone this project inside the directory you want to build.
* Generate build system from source files.
  * Open **CMake**
  * Type in the source path
  * Type in the build path
  * Press **Configure** button
  * Select **Visual Studio 15 2017** as generator
  * Select **x64** as an optional platform for generator
  * Press **Finish** button
  * Press **Build** button
* Run solution file
  * Open the solution file
  * Set **Application** as **Startup Project**
  * Build and Run


## Documentation
All descriptions are directly written to the code itself.

## Copyright
Copyright © 2019 Hajun Kim, Undergraduate student in KAIST. All rights reserved. 
