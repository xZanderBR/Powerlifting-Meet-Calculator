# Powerlifting Meet Calculator
## Overview
**Powerlifting Meet Calculator** is designed for managing powerlifting meets efficiently across platforms. Users are able to manage lifters, calculate scores using popular formulas, and import/export data with ease. It provides a smooth and responsive graphical user interface through ImGui with Vulkan for rendering.

## Features
- Lifter Management: Add, update, and sort lifters based on various attributes.
- Score Calculations: Supports [DOTS](https://drive.google.com/file/d/1GG22jYL3JaalTUydcigkZNCjddtH-0rv/view), [IPF](https://www.powerlifting.sport/fileadmin/ipf/data/ipf-formula/Models_Evaluation-I-2020.pdf), [IPF GL](https://www.powerlifting.sport/fileadmin/ipf/data/ipf-formula/IPF_GL_Coefficients-2020.pdf), [Wilks](https://www.powerlifting.sport/fileadmin/ipf/data/ipf-formula/Models_Evaluation-I-2020.pdf), and [Wilks-2](https://worldpowerlifting.com/wilks-formula/) formulas.
- File Management: Import and export meet data in JSON format.
- Asynchronous Tasks: Perform IO operations and sorting in the background to maintain a responsive UI.
- Multithreaded Sorting: Larger datasets utilize a custom multithreaded sorting algorithm.
- Key Binding: Hotkeys to open and save meet files through simple keystroke combinations.
- Cross-Platform: All features work seamlessly on Windows and Linux (MacOS untested).

## Screenshots
### Windows:
![Powerlifting-Meet-Calculator-win](https://github.com/user-attachments/assets/3e99fd69-2eaa-49db-af12-33b3d7c42e6b)

### Linux:
![Powerlifting-Meet-Calculator-linux](https://github.com/user-attachments/assets/6c603b68-a832-435f-ad7f-9edec464c554)


## Installation

### Prebuilt Binaries
The latest Windows builds are compiled with MSVC and posted under [releases](https://github.com/xZanderBR/Powerlifting-Meet-Calculator/releases). Alternatively, you can compile the code from the source yourself with the instructions below:

### Compilation
#### Prerequisites
- C++20 compiler (GCC, Clang, MSVC)
- CMake (minimum version 3.29)
- [Vulkan SDK](https://vulkan.lunarg.com/) (with VULKAN_SDK environment variable set to the Vulkan SDK path)

#### Building
1. Clone the repository:
   ```bash
   git clone https://github.com/xZanderBR/Powerlifting-Meet-Calculator
   cd Powerlifting-Meet-Calculator
   ```
2. Configure and build the project:
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release
   ```
3. Run the program:
   ```bash
   ./Powerlifting-Meet-Calculator
   ```

**NOTE:** When compiling with MinGW under Windows, it may be required to statically link libstdc++ to avoid crashes during filesystem operations on release builds. To do so, add the line below to the main `CMakeLists.txt`:
```bash
target_link_options(Powerlifting-Meet-Calculator PRIVATE -static-libstdc++)
```

## Dependencies
This project uses the following dependencies:
- [nlohmann/json](https://github.com/nlohmann/json): JSON serialization and deserialization.
- [Dear ImGui](https://github.com/ocornut/imgui): Graphical user interface.
- [GLFW](https://github.com/glfw/glfw): Window and input management.
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog): File selection dialog for ImGui.

Dependencies are automatically fetched using CMake's `FetchContent`.

## License
This project is licensed under the MIT License. For more details, see the [LICENSE](https://github.com/xZanderBR/Powerlifting-Meet-Calculator/blob/main/LICENSE) file.
