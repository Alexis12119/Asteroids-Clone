cmake_minimum_required(VERSION 3.11)
project(raylib-asteroids C)

set(CMAKE_C_STANDARD 99)

# Find Raylib
find_package(raylib REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} src/main.c src/player.c src/asteroid.c src/bullet.c src/menu.c src/leaderboard.c src/settings.c)

# Link Raylib and math
target_link_libraries(${PROJECT_NAME} raylib m)