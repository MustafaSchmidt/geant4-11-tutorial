cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Tutorial)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)

add_executable(sim sim.cc ${sources})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(Tutorial DEPENDS sim)