cmake_minimum_required(VERSION 3.10)
project(topk_words)

set(CMAKE_CXX_STANDARD 17)

# Для MSVC убираем все флаги по умолчанию и ставим свои
if(MSVC)
    # Очищаем флаги по умолчанию
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    
    # Ручная настройка
    add_compile_options(/W4 /MP /EHsc /std:c++17)
    
    # Разные флаги для Debug и Release
    add_compile_options(
        "$<$<CONFIG:Release>:/O2 /MD>"
        "$<$<CONFIG:Debug>:/Od /MDd /RTC1>"
    )
endif()

add_executable(topk_words optimized_topk_words.cpp)
target_link_libraries(topk_words)