cmake_minimum_required(VERSION 3.27)

project(Primer1_03_FlyingLine)

#set(CMAKE_C_STANDARD 11)

include($ENV{CMAKE_TOOLCHAIN_RETRO68})

set(CMAKE_TOOLCHAIN_FILE, $ENV{CMAKE_TOOLCHAIN_RETRO68})

#include_directories($ENV{RETRO68_INTERFACE_CINCLUDES})
#include($ENV{RETRO68_INTERFACE_CINCLUDES})

# cmake .. -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_RETRO68

add_compile_definitions(
        OLDROUTINENAMES=1
)


add_application(Primer1_03_FlyingLine
#        TYPE "APPL"
#        CREATOR "R68L"
#        ../../ThinkCHelper/ThinkHelpers.h
        src/FlyingLine.c
        CONSOLE
)

set_target_properties("Primer1_03_FlyingLine" PROPERTIES C_STANDARD 90)

