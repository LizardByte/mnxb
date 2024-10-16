cmake_minimum_required(VERSION 3.14)
# much of this file is borrowed from https://github.com/Ryzee119/Xenium-Tools/blob/master/CMakeLists.txt

project(Moonlight C CXX)
set(XBE_TITLE ${CMAKE_PROJECT_NAME})
message(STATUS "XBE_TITLE: ${XBE_TITLE}")

set(NXDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third-party/nxdk")
message(STATUS "NXDK_DIR: ${NXDK_DIR}")

set(XBOX_XBE_DIR "${CMAKE_CURRENT_BINARY_DIR}/xbe")
set(XBOX_ISO "${CMAKE_PROJECT_NAME}.iso")
message(STATUS "XBOX_ISO: ${XBOX_ISO}")

#
# Options
#
option(BUILD_TESTS "Build tests" OFF)

# create the xbe directory if it doesn't exist
file(MAKE_DIRECTORY ${XBOX_XBE_DIR})

include(FindPkgConfig)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")

set(MOONLIGHT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)
set(MOONLIGHT_EXTERNAL_LIBRARIES
        "${NXDK_DIR}/lib/libpbkit.lib"
)
set(MOONLIGHT_INCLUDE_DIRS
        "${NXDK_DIR}/lib"
)
set(MOONLIGHT_COMPILE_OPTIONS "-Wno-builtin-macro-redefined")
set(MOONLIGHT_DEFINITIONS "-DXBOX -DNXDK")

# Stop lots of warning spam
add_compile_options(${MOONLIGHT_COMPILE_OPTIONS})
add_definitions(${MOONLIGHT_DEFINITIONS})

add_executable(${CMAKE_PROJECT_NAME}
        ${MOONLIGHT_SOURCES}
)
include_directories(SYSTEM ${MOONLIGHT_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
        ${MOONLIGHT_EXTERNAL_LIBRARIES}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE "")

#
# tests
#
if(BUILD_TESTS)
    add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()

# Post-build exe to xbe conversion
add_custom_target(cxbe_convert ALL
        VERBATIM COMMAND "${CMAKE_COMMAND}" -E env
            ./tools/cxbe/cxbe
            -OUT:${XBOX_XBE_DIR}/default.xbe
            -TITLE:${XBE_TITLE}
            ${CMAKE_CURRENT_BINARY_DIR}/${XBE_TITLE}.exe
        WORKING_DIRECTORY ${NXDK_DIR}
        COMMENT "CXBE Conversion: [EXE -> XBE]"
)
add_dependencies(cxbe_convert ${CMAKE_PROJECT_NAME})

# Post-build xbe to xiso conversion
add_custom_target(xbe_iso ALL
        VERBATIM COMMAND "${CMAKE_COMMAND}" -E env
            ${NXDK_DIR}/tools/extract-xiso/build/extract-xiso
            -c ${XBOX_XBE_DIR} ${XBOX_ISO}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "CXBE Conversion: [XBE -> XISO]"
)
add_dependencies(xbe_iso cxbe_convert)

set_target_properties(cxbe_convert PROPERTIES OUTPUT_QUIET ON)
set_target_properties(xbe_iso PROPERTIES OUTPUT_QUIET ON)
