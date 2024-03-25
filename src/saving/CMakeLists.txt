SET(NAME Saving)
SET(SOURCES saveResults.cpp saveNormResidual.cpp vtkOutput.cpp)

ADD_LIBRARY(${NAME} ${SOURCES})
