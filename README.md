# OpenCL Ray Tracing Engine

A simple raytracing engine written in C++ using OpenCL for GPU accelerated parallel compute and OpenGL for presentation.

## Demo

The following image shows raytracing using distance-functions for simple 3D geometry i.e plane, sphere, ray intersections. This has been extended to create simple shadows and diffuse lighting.

![Raytrace Rendering](/res/img/demo/raytrace_test.png)

The engine also supports the classical OpenGL rendering pipeline using vertex data loaded from Wavefront (.obj) files and GLSL shaders.

![Classic Rendering](/res/img/demo/raster_test.png)

## Building

To build the engine source code, the following dependencies must be satisfied and then the `make` command should be run in a terminal. The source code was written and developed on a 64-bit Windows machine so heavy modification may be required to adapt it for your system.

### Dependencies

+ A GPU that supports

  + OpenCL version 3.0 or higher

  + OpenGL version 3.2 or higher

+ NVIDIA GPU Computing Toolkit (Edit the OPENCL path in the [CMakeLists.txt](/CMakeLists.txt) file)

+ Download the correct binaries and headers for your system:

  + GLFW


  + GLEW

  + stb  - [headers only](https://github.com/nothings/stb)

  + glm  - [headers only](https://github.com/g-truc/glm/tree/master/glm)

  and place them in the [include](/include/) and [libs](/libs/) directories in a subdirectory with the name of the library.

+ GNU Make and CMake

+ Visual Studio Build Tools 2022