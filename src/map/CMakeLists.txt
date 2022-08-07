set(LIBRARY_NAME map)
set(SOURCES
        camera.cpp
        game_map.cpp
        map_layer.cpp
        map_point.cpp
        grid/grid_neighbors.cpp
        grid/grid_function.cpp
        surface/diamond_square.cpp
        surface/perlin.cpp
        surface/surface_layer.cpp
        underground/underground_layer.cpp
        )
add_library(${LIBRARY_NAME} "${SOURCES}")