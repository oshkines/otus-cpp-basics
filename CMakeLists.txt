cmake_minimum_required(VERSION 3.15)

# КРИТИЧЕСКИ ВАЖНО: Эта строка должна быть ПЕРВОЙ, до project()!
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(PqxxProject LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем libpqxx через CONFIG mode
find_package(libpqxx CONFIG REQUIRED)

set(SOURCES
    main.cpp
    readSQL.cpp
    consoleOutput.cpp
    deleteSQL.cpp
    updataSQL.cpp
    utils.cpp
    dbconfig.cpp         
    connectionmanager.cpp
)

# Список заголовочных файлов
set(HEADERS
    common.hpp
    readSQL.hpp
    consoleOutput.hpp
    deleteSQL.hpp
    updataSQL.hpp
    utils.hpp
    dbconfig.hpp         
    connectionmanager.hpp
)
# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Линкуем библиотеку
target_link_libraries(${PROJECT_NAME} PRIVATE 
    libpqxx::pqxx
)

# Для Windows
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _WIN32_WINNT=0x0A00
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

# Информация для отладки
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "VCPKG_TOOLCHAIN: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Found libpqxx: TRUE")