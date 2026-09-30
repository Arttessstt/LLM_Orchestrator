cmake_minimum_required(VERSION 3.20)
project(LLM_Orchestrator)

# Устанавливаем стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(LLM_Orchestrator src/main.cpp
        include/Types.hpp
        include/IProvider.hpp)