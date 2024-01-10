cmake_minimum_required(VERSION 3.27)
project(macintosh_c_programming_primer_2nd_vol_1_05_WorldClock)

#set(CMAKE_C_STANDARD 11)

include($ENV{CMAKE_TOOLCHAIN_RETRO68})

include_directories($ENV{RETRO68_INTERFACE_CINCLUDES})


add_executable(macintosh_c_programming_primer_2nd_vol_1_05_WorldClock
        src/WorldClock.c

)

