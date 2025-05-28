cmake_minimum_required(VERSION 3.31)
project(OzzylogicQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

qt_add_executable(OzzylogicQt ${SOURCES} ${HEADERS})

target_include_directories(OzzylogicQt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(OzzylogicQt PUBLIC Qt6::Core Qt6::Widgets)

add_custom_target(copy_database ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
        COMMENT "Copying database into binary directory")

add_dependencies(OzzylogicQt copy_database)