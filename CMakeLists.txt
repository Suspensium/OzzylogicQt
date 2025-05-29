cmake_minimum_required(VERSION 3.31)
project(OzzylogicQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
qt_standard_project_setup()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

qt_add_executable(OzzylogicQt ${SOURCES} ${HEADERS})

target_include_directories(OzzylogicQt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(OzzylogicQt PUBLIC Qt6::Core Qt6::Widgets Qt6::Sql)

# Copy database directory
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/data/copied.flag
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/data/copied.flag
        DEPENDS ${CMAKE_SOURCE_DIR}/data
        COMMENT "Copying database into binary directory if changed"
)

add_custom_target(copy_database ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/data/copied.flag
)

# Copy icons directory
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icons/copied.flag
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/icons ${CMAKE_CURRENT_BINARY_DIR}/icons
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/icons/copied.flag
        DEPENDS ${CMAKE_SOURCE_DIR}/icons
        COMMENT "Copying icons into binary directory if changed"
)

add_custom_target(copy_icons ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/icons/copied.flag
)

add_dependencies(OzzylogicQt copy_database copy_icons)