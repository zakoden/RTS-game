set(LIBRARY_NAME behaviors)
set(SOURCES
        behavior_hunter.cpp
        behavior_bullet.cpp
        behavior_around_point.cpp
        behavior.cpp
        behavior_random.cpp
        behavior_stay.cpp
        behavior_tower.cpp
        behavior_scout.cpp
        )
add_library(${LIBRARY_NAME} "${SOURCES}")
