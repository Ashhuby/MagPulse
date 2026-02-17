cmake_minimum_required(VERSION 3.11)
project(BounceLab)  

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-3.0.2/lib/cmake/SFML")
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# FIRST: Create the target
add_executable(BounceLab src/main.cpp)

# SECOND: Add features and options to the existing target
target_compile_features(BounceLab PRIVATE cxx_std_17)

# THIRD: Add MSVC specific flags (if needed)
if(MSVC)
    target_compile_options(BounceLab PRIVATE /Zc:__cplusplus)
endif()

# FOURTH: Link libraries
target_link_libraries(BounceLab PRIVATE SFML::Graphics SFML::Window SFML::System)

# Copy DLLs
if(WIN32)
    add_custom_command(TARGET BounceLab POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML-3.0.2/bin/sfml-graphics-d-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML-3.0.2/bin/sfml-window-d-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML-3.0.2/bin/sfml-system-d-3.dll"
        $<TARGET_FILE_DIR:BounceLab>
    )
endif()