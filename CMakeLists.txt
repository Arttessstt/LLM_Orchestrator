cmake_minimum_required(VERSION 3.20)
project(LLM_Orchestrator)

# Устанавливаем стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

include_directories(include third_party)

add_executable(LLM_Orchestrator src/main.cpp
        include/Types.hpp
        include/IProvider.hpp
        include/CloudProvider.hpp
        src/CloudProvider.cpp
        include/FallbackOrchestrator.hpp
        src/FallbackOrchestrator.cpp)

# 3. Линкуем проект с сетью и новыми библиотеками шифрования
if (WIN32)
    # Возвращаем указание компилятору, что мы на Windows 10
    target_compile_definitions(LLM_Orchestrator PRIVATE _WIN32_WINNT=0x0A00 WINVER=0x0A00)
    target_link_libraries(LLM_Orchestrator PRIVATE ws2_32 crypt32 ssl crypto)
else()
    target_link_libraries(LLM_Orchestrator PRIVATE ssl crypto)
endif()

# 1. Автоматически скачиваем и собираем LibreSSL (аналог OpenSSL)
include(FetchContent)
FetchContent_Declare(
        libressl
        URL https://cdn.openbsd.org/pub/OpenBSD/LibreSSL/libressl-3.8.2.tar.gz
)
# Отключаем лишние компоненты, чтобы ускорить сборку
set(LIBRESSL_APPS OFF CACHE BOOL "" FORCE)
set(LIBRESSL_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(libressl)

# 2. Включаем поддержку HTTPS в httplib
target_compile_definitions(LLM_Orchestrator PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

# 3. Линкуем проект с сетью и новыми библиотеками шифрования (ssl и crypto)
if (WIN32)
    target_link_libraries(LLM_Orchestrator PRIVATE ws2_32 ssl crypto)
else()
    target_link_libraries(LLM_Orchestrator PRIVATE ssl crypto)
endif()