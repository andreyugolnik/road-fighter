project(roadfighter)
cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)
include(CheckLibraryExists)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    message(STATUS "=== RELEASE MODE ===")
    add_definitions( "-DNDEBUG" )
    add_definitions( "-Wall -O2" )
else()
    message(STATUS "=== DEBUG MODE ===")
    add_definitions( "-DDEBUG" )
    add_definitions( "-Wall -Wextra -pedantic -O0 -g" )
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

find_package(PkgConfig)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_ttf REQUIRED)

include_directories(
    ${SDL_INCLUDE_DIRS}
    "src"
    )

link_directories(
    ${SDL_LIBRARY_DIRS}
    )

file( GLOB_RECURSE GAME_SOURCES "src/*.cpp" )
file( GLOB_RECURSE GAME_RESOURCES "graphics/*" )

# set(APP_TITLE           "Road Fighter" CACHE STRING "Application name")
# set(APP_VERSION_MAJOR   "1" CACHE STRING "Application major version")
# set(APP_VERSION_MINOR   "0" CACHE STRING "Application minor version")
# set(APP_VERSION_RELEASE "0" CACHE STRING "Application release version")
set(VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}${APP_VERSION_RELEASE}")
message(STATUS "Version ${VERSION}")

# if(APPLE)
    # set(GAME_RESOURCES_PREFIX "/Users/andrey/projects/External/roadfighter-1.0.1269/roadfighter.app/Contents/Resources/" CACHE STRING "Resources prefix")
# else()
    # set(GAME_RESOURCES_PREFIX "" CACHE STRING "Resources prefix")
# endif()
# configure_file(
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/Config.h.in"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/Config.h"
    # )

if(APPLE)
    list( REMOVE_ITEM GAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Assets.cpp")
    set(GAME_SOURCES ${GAME_SOURCES} "src/Assets.mm")

    set(GAME_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/Icon.icns)
    set_source_files_properties(${GAME_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    # set( GAME_RESOURCES "graphics/*" "fonts/*" )

    add_executable(roadfighter MACOSX_BUNDLE ${GAME_ICON} ${GAME_SOURCES} ${GAME_RESOURCES})

    set_target_properties(roadfighter PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/Info.plist.in
        MACOSX_BUNDLE_LONG_VERSION_STRING ${VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION}
        )
else()
    add_executable(roadfighter ${GAME_SOURCES})
endif()

target_link_libraries( roadfighter
    ${SDL_LIBRARY}
    ${SDL_IMAGE_LIBRARY}
    ${SDL_MIXER_LIBRARY}
    ${SDL_TTF_LIBRARY}
    )
