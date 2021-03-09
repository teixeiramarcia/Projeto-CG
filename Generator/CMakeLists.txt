cmake_minimum_required(VERSION 3.9)

project(generator)

aux_source_directory(src source)

add_executable(generator main.cpp ${source})
