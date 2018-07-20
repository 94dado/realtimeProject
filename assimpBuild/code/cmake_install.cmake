# Install script for directory: D:/Desktop/progettoGrafica/include/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Desktop/progettoGrafica/assimpBuild/code/Debug/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Desktop/progettoGrafica/assimpBuild/code/Release/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Desktop/progettoGrafica/assimpBuild/code/MinSizeRel/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Desktop/progettoGrafica/assimpBuild/code/RelWithDebInfo/assimp-vc140-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp3.3.1x" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Desktop/progettoGrafica/assimpBuild/code/Debug/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Desktop/progettoGrafica/assimpBuild/code/Release/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Desktop/progettoGrafica/assimpBuild/code/MinSizeRel/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Desktop/progettoGrafica/assimpBuild/code/RelWithDebInfo/assimp-vc140-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/anim.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/ai_assert.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/camera.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/color4.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/color4.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/config.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/defs.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/cfileio.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/light.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/material.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/material.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/matrix3x3.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/matrix3x3.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/matrix4x4.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/matrix4x4.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/mesh.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/postprocess.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/quaternion.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/quaternion.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/scene.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/metadata.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/texture.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/types.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/vector2.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/vector2.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/vector3.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/vector3.inl"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/version.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/cimport.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/importerdesc.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/Importer.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/IOStream.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/IOSystem.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/Logger.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/LogStream.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/NullLogger.hpp"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/cexport.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/Exporter.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/Desktop/progettoGrafica/include/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Desktop/progettoGrafica/assimpBuild/code/Debug/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Desktop/progettoGrafica/assimpBuild/code/RelWithDebInfo/assimp.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

