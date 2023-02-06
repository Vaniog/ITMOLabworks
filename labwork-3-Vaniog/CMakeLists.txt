cmake_minimum_required(VERSION 3.22)
project(labwork_3_Vaniog)

set(CMAKE_CXX_STANDARD 17)

include_directories(BMP_Manager)

add_executable(labwork_3_Vaniog
        BMP_Manager/WriteAsBMP.h
        BMP_Manager/WriteAsBMP.cpp
        BMP_Manager/BMP_Structs.h
        BMP_Manager/BMP_Structs.cpp
        Sand_Heap/SandHeap.h
        Sand_Heap/SandHeap.cpp
        main.cpp Sand_Heap/SandData.h)
