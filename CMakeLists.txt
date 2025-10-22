cmake_minimum_required(VERSION 3.11)
project(Geodroid)

set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(src)

# Find Raylib
find_package(raylib REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} src/main.c src/player.c src/asteroid.c src/bullet.c src/menu.c src/leaderboard.c src/settings.c)

# Copy assets to build directory
configure_file(${CMAKE_SOURCE_DIR}/assets/laser.wav ${CMAKE_BINARY_DIR}/assets/laser.wav COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/explosion.wav ${CMAKE_BINARY_DIR}/assets/explosion.wav COPYONLY)

# Link Raylib and math
target_link_libraries(${PROJECT_NAME} raylib m)
