cmake_minimum_required(VERSION 3.11.2)
project(fst_example)

set(CMAKE_CXX_STANDARD 11)

include_directories(openfst-1.7.2/src/include)
file(GLOB_RECURSE openfst_src openfst-1.7.2/src/lib/*.cc)


add_executable(fst_example
        main.cxx
        ${openfst_src})

add_executable(fst_dic
        dic.cxx
        ${openfst_src})

add_executable(fst_dic2
        dic2.cxx
        ${openfst_src})