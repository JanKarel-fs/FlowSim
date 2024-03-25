SET(NAME Sources)
SET(SOURCES initialisation.cpp inlet.cpp outlet.cpp
setBoundaries.cpp setting.cpp timeStep.cpp wall.cpp)

ADD_LIBRARY(${NAME} ${SOURCES})
