# Antic_Graph


Graphics API made to be used with [Antic_Engine](https://github.com/shanesatterfield/Antic_Engine). This is a simple C++11 graphics API that uses OpenGL 2.1 for rendering and SDL2 for window management.

## Dependencies

- [SDL2](https://www.libsdl.org/download-2.0.php)
- OpenGL 2.1+
- [GLEW](http://glew.sourceforge.net/)
- [DevIL](http://openil.sourceforge.net/download.php) (Developer's Image Library also known as OpenIL)
- [CMake](http://www.cmake.org/download/)
- C++11
- RapidJSON

## Getting Started

### Compiling the Source Code

To compile the source code, download the dependencies listed above onto your machine as well as the source code itself. From here, run CMake inside the repository. I good practice with CMake is to create a build directory within the root of the project and run CMake from within there. An example of this is as follows.

```
cd /path/to/Antic_Graph/
mkdir build/
cd build/
cmake .. && make
```

This will result in libAntic_Graph.a being compiled. You can find this static library in the same directory that you ran the make command in.