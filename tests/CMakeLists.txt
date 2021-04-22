cmake_minimum_required(VERSION 3.12)

if (UNIX)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif (UNIX)
if(WIN32)
set(CMAKE_C_COMPILER "cl")
set(CMAKE_CXX_COMPILER "cl")
endif (WIN32)

project(Reflection_Tests VERSION 1.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB_RECURSE Reflection_Tests_SRC
"src/*.cpp"
"src/*.hpp"
"src/*.h")


add_executable(${PROJECT_NAME} ${Reflection_Tests_SRC})

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "../include"
    "include"
    "scripts/Generated"
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/scripts/)
if (DEFINED HOT_RELOAD)
message(WARNING "HOT RELOAD")
    add_definitions(-DHOT_RELOAD)
    if (UNIX)
        target_link_libraries(${PROJECT_NAME}
        dl)
    endif (UNIX)
    if (WIN32)
         if (${CMAKE_GENERATOR} MATCHES "Ninja")
            add_definitions(-DMS_BUILD_BIN="ninja") 
         else()
            add_definitions(-DMS_BUILD_BIN="$(MSBuildBinPath)")    
            add_definitions(-DBUILD_CONFIG="$(Configuration)")  
         endif(${CMAKE_GENERATOR} MATCHES "Ninja")   
    endif (WIN32)
else()
message(WARNING "NO HOT RELOAD")
    target_link_libraries(${PROJECT_NAME}
    Scripts)
    endif(DEFINED HOT_RELOAD)
message(WARNING ${CMAKE_GENERATOR})
if (${CMAKE_GENERATOR} MATCHES "Ninja")
    add_definitions(-DCOMPILER=ninja)
    message(Warning "USING NINJA")
elseif (${CMAKE_GENERATOR} MATCHES "Make")
    add_definitions(-DCOMPILER=make)
message(Warning "USING MAKE")
elseif (${CMAKE_GENERATOR} MATCHES "Visual Studio")
    #add_definitions(-DCOMPILER=msbuild)
message(Warning "USING VS")
elseif (${CMAKE_GENERATOR} MATCHES "Xcode")
    add_definitions(-DCOMPILER=xcode)
message(Warning "XCODE")
endif (${CMAKE_GENERATOR} MATCHES "Ninja")

