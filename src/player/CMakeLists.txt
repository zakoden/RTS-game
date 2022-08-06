set(LIBRARY_NAME player)
set(SOURCES
        player.cpp
        players_info.cpp
        )
create_library(${LIBRARY_NAME} "${SOURCES}")