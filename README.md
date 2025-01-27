# SDL3-BGFX Starter C Project

This project is a starter template for using SDL3 with BGFX in C. It includes basic examples for rendering a clear screen, a triangle, and a quad.

## Getting Started

### Prerequisites

- CMake
- A C/C++ compiler

### Building the Project

1. Clone the repository:
    ```sh
    git clone git@github.com:seemsindie/sdl3-bgfx-starter-C.git
    cd sdl3-bgfx-starter-C
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make -j8
    ```

### Running the Examples

After building the project, you can run the examples:

- Clear screen example:
    ```sh
    ./sdl3-bgfx-clear
    ```

- Triangle example:
    ```sh
    ./sdl3-bgfx-triangle
    ```

- Quad example:
    ```sh
    ./sdl3-bgfx-quad
    ```