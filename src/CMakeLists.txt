cmake_minimum_required(VERSION 3.22)

project(rts-game)

set(CMAKE_CXX_STANDARD 17)

# Include .h files in SDL to allow include <SDL.h>
include_directories(SDL2 ${PROJECT_SOURCE_DIR}/SDL2/include/SDL2)
# Link libraries to solve errors like "undefined reference to `SDL_Init'"
link_directories(${PROJECT_SOURCE_DIR}/SDL2/lib)

set(LIBRARIES
        managers
        map
        utils
        player
        units
        behaviors)

set(SOURCES
        adapter.cpp
        clock.cpp
        main.cpp)

set(PICTURES
        pictures/bamboo.bmp
        pictures/chest.bmp
        pictures/fire_medium_poleax.bmp
        pictures/fire_medium_sword.bmp
        pictures/fire_small_lance.bmp
        pictures/fire_small_poleax.bmp
        pictures/fire_small_spear.bmp
        pictures/map-tiles.bmp
        pictures/map-tiles-fogged.bmp
        pictures/small_gray_tower.bmp
        pictures/small_horizontal_gray_wall.bmp
        pictures/small_vertical_gray_wall.bmp
        pictures/testunit.bmp
        pictures/testunit1.bmp
        pictures/testunit2.bmp
        pictures/testunit2_1.bmp
        pictures/testunit3.bmp)

foreach(pic IN ITEMS ${PICTURES})
    message("Adding picture ${pic}")
    configure_file(${pic} ${pic} COPYONLY)
endforeach()

foreach(lib IN ITEMS ${LIBRARIES})
    message("Adding library ${lib}")
    add_subdirectory(${lib})
endforeach()

add_executable(rts-game ${SOURCES})
target_link_libraries(rts-game ${MAP_LIBRARIES} ${LIBRARIES} SDL2 SDL2main)

add_definitions(-DSDL_MAIN_HANDLED)
