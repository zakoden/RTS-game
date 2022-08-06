set(LIBRARY_NAME managers)
set(SOURCES
        game_manager.cpp
        texture_manager.cpp
        user_manager.cpp
        )

add_library(${LIBRARY_NAME} ${SOURCES})
