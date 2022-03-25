# minimal cmake version
cmake_minimum_required(VERSION 3.20)

# fallback for other versions 
if(${CMAKE_VERSION} VERSION_LESS 3.20)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.20) 
endif()

message("generator: ${CMAKE_GENERATOR}")

# set the cxx compiler standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# the project
project(Alloc DESCRIPTION "The Egel allocator prototype"
        LANGUAGES CXX)

# set the cxx compiler flags
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# compilers
message("compiler: ${CMAKE_CXX_COMPILER_ID}")
if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
  set(CMAKE_CXX_FLAGS                     "-Wall -Wextra -Werror -pedantic -Wno-unused-parameter -Wno-return-type-c-linkage")
  set(CMAKE_CXX_FLAGS_DEBUG               "-g")
  set(CMAKE_CXX_FLAGS_RELEASE             "-Ofast")

  set(CMAKE_CXX_FLAGS_PROFILE             "-Ofast -fprofile-instr-generate -fcoverage-mapping")
  #set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-O3 -pg")
  #set(CMAKE_SHARED_LINKER_FLAGS_PROFILE   "-O3 -pg")
elseif(("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang"))
  set(CMAKE_CXX_FLAGS                     "-Wall -Wextra -Werror -pedantic -Wno-unused-parameter -Wno-return-type-c-linkage")
  set(CMAKE_CXX_FLAGS_DEBUG               "-g")
  set(CMAKE_CXX_FLAGS_RELEASE             "-Ofast")

  set(CMAKE_CXX_FLAGS_PROFILE             "-O3 -pg")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-O3 -pg")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE   "-O3 -pg")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS                     "-Wall -Wextra -Werror -pedantic -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS_DEBUG               "-g")
  set(CMAKE_CXX_FLAGS_RELEASE             "-O3")

  set(CMAKE_CXX_FLAGS_PROFILE             "-O3 -pg")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-O3 -pg")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE   "-O3 -pg")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(CMAKE_CXX_FLAGS                     "-Wall -Wextra -Werror -pedantic -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS_DEBUG               "-g")
  set(CMAKE_CXX_FLAGS_RELEASE             "-O3")

  set(CMAKE_CXX_FLAGS_PROFILE             "-O3 -pg")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-O3 -pg")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE   "-O3 -pg")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS                     "-Wall -Wextra -Werror -pedantic -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS_DEBUG               "-g")
  set(CMAKE_CXX_FLAGS_RELEASE             "-O3")

  set(CMAKE_CXX_FLAGS_PROFILE             "-O3 -pg")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-O3 -pg")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE   "-O3 -pg")
else()
  set(CMAKE_CXX_FLAGS                     "-Wall -Wextra -Werror -pedantic -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS_DEBUG               "-g")
  set(CMAKE_CXX_FLAGS_RELEASE             "-O3")

  set(CMAKE_CXX_FLAGS_PROFILE             "-O3 -pg")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-O3 -pg")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE   "-O3 -pg")
endif()

# for the moment, glob sources and headers
file(GLOB EGEL_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB EGEL_HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.hpp")

# check for ICU, Threads, fmt
# ICU components: data, i18n, io, le, lx, test, tu and uc.
find_package(ICU 60.0 REQUIRED COMPONENTS data i18n io tu uc)
find_package(Threads REQUIRED)
# find_package(mimalloc REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/src")

# objects
add_library(objlib OBJECT ${EGEL_SOURCES})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(objlib Threads::Threads ICU::uc ICU::i18n ICU::io)

# the Egel interpreter
add_executable(test $<TARGET_OBJECTS:objlib>)
target_link_libraries(test ${CMAKE_DL_LIBS} Threads::Threads ICU::uc ICU::i18n ICU::io)

