set(LIBRARY_NAME units)
set(SOURCES
        building.cpp
        drawable_entity.cpp
        entity.cpp
        unit.cpp
        unit_factory.cpp
        immovable_unit.cpp
        )
create_library(${LIBRARY_NAME} "${SOURCES}")