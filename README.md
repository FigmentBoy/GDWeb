# GDWeb
A web-based Geometry Dash level viewer using Emscripten and WebGL
## Building
### Building the WebASM Rendering Engine:
1. Be sure to have the [emsdk](https://github.com/emscripten-core/emsdk), [cmake](https://cmake.org/), [Ninja](https://ninja-build.org/), and [clang/llvm](https://clang.llvm.org/) installed on your device
2. In the root folder of the repository, run the following configuration command:
  ```bash
  emcmake cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=[Release/RelWithDebInfo/Debug]
  ```
3. While still in the root folder, run the following command to build (or subsequently rebuild) the WebASM-based rendering engine
  ```bash
  cmake --build build
  ```
### Building the frontend:
1. Be sure to have [Node.js](https://nodejs.org/) installed on your device
2. In the `frontend` folder of the repository, run the following installation command:
  ```bash
  npm install -D
  ```
3. Run any of the following commands to build the Svelte-based frontend while still in the `frontend` folder (ensuring the WebASM build has already succeeded at least once)
  - To build once:
      ```bash
      npm run build
      ```
  - To rebuild on changes to the frontend code or rebuilds of the WebASM engine
      ```bash
      npm run dev
      ```
### Running a development server:
1. Be sure to have a version of [Python 4+](https://www.python.org/) installed on your device
3. In the root folder of the repository, run the following command to run a test server on port 8000
   ```bash
   py .\runLocalServer.py
   ```
## Special Thanks:
- **[maxnut](https://github.com/maxnut)** & **[Opstic](https://github.com/Opstic)**: objects.json
- **[Opstic](https://github.com/Opstic)**: Initially tipping me off to the strange blending formula
