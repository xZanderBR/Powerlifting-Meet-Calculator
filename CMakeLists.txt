cmake_minimum_required(VERSION 3.29)
project(Powerlifting-Meet-Calculator)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

# Fetch nlohmann/json
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Fetch Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.4
)
FetchContent_MakeAvailable(imgui)

# Fetch GLFW
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Fetch ImGuiFileDialog
FetchContent_Declare(
        ImGuiFileDialog
        GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git
        GIT_TAG master
)
FetchContent_MakeAvailable(ImGuiFileDialog)

# Fetch GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find Vulkan and Threads
find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)

include_directories(
        ${glfw_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/misc/cpp
        ${imgui_SOURCE_DIR}/backends
        ${json_SOURCE_DIR}/include
        ${ImGuiFileDialog_SOURCE_DIR}
)

# Gather source files
file(GLOB sources *.cpp)

add_executable(Powerlifting-Meet-Calculator
        ${sources}
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
        ui/vulkan_setup.h
        ui/vulkan_setup.cpp
        Meet.h
        Lifter.h
        enums.h
        Meet.cpp
        ui/MeetWindowState.h
        ui/MeetWindow.cpp
        ui/MeetWindow.h
        ui/IMGUITHEME.h
        ui/IMGUITHEME.cpp
        Serialization.h
        Serialization.cpp
        MeetIO.cpp
        MeetIO.h
)

# Configure Windows Subsystem
if(WIN32)
    if(MSVC)
        target_link_options(Powerlifting-Meet-Calculator PRIVATE
                $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
                $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(Powerlifting-Meet-Calculator PRIVATE
                $<$<CONFIG:Release>:-mwindows>
                $<$<CONFIG:Debug>:-mconsole>
        )
    endif()
endif()

target_include_directories(ImGuiFileDialog PRIVATE ${imgui_SOURCE_DIR})
target_link_libraries(Powerlifting-Meet-Calculator
        PRIVATE
        glfw
        Vulkan::Vulkan
        ImGuiFileDialog
        Threads::Threads
)
target_compile_definitions(Powerlifting-Meet-Calculator PUBLIC -DImTextureID=ImU64)

enable_testing()
add_subdirectory(tests)
