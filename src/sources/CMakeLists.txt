SET(NAME Sources)
SET(SOURCES initialisation.cpp inlet.cpp outlet.cpp
setBoundaries.cpp setting.cpp timeStep.cpp slipWall.cpp noSlipWall.cpp
symmetry.cpp homogeneousNeumann.cpp setGrid.cpp inletJacobian.cpp
outletJacobian.cpp slipWallJacobian.cpp noSlipWallJacobian.cpp
symmetryJacobian.cpp homogeneousNeumannJacobian.cpp)

ADD_LIBRARY(${NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${NAME} Fvm)
