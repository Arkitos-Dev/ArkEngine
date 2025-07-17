# Install script for directory: C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/3DRenderer")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/JetBrains/CLion 2025.1.1/bin/mingw/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-build/code/libassimp.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/anim.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/ai_assert.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/camera.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/color4.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/color4.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/config.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/defs.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/cfileio.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/light.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/material.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/material.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/matrix3x3.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/matrix3x3.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/matrix4x4.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/matrix4x4.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/mesh.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/postprocess.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/quaternion.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/quaternion.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/scene.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/metadata.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/texture.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/types.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/vector2.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/vector2.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/vector3.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/vector3.inl"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/version.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/cimport.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/importerdesc.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/Importer.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/DefaultLogger.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/ProgressHandler.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/IOStream.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/IOSystem.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/Logger.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/LogStream.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/NullLogger.hpp"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/cexport.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/Exporter.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/Compiler/pushpack1.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/Compiler/poppack1.h"
    "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-src/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/anton/CLionProjects/3DRenderer/cmake-build-release/_deps/assimp-build/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
