cmake_minimum_required(VERSION 3.27)
project(2_Plane)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем предупреждения для компилятора
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

# Указываем путь к директории с исходниками
include_directories(units)

# Список всех исходных файлов проекта
set(SOURCES
        main.cpp
        units/unit.hpp
        units/crew_members.cpp
        units/crew_members.hpp
        units/passengers.cpp
        units/passengers.hpp
        units/plane.cpp
        units/plane.hpp
        units/unit.cpp
)

# Создаём исполняемый файл
add_executable(2_Plane ${SOURCES})