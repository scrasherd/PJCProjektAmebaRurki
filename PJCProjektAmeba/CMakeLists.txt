cmake_minimum_required(VERSION 3.15)
project(PJCProjektAmebaRurki)

include_directories("C:/SFML/source/repos/PJCProjektAmebaRurki/External/include")

set(CMAKE_CXX_STANDARD 20)

# Szukaj wszystkich plików .cpp i .h w bieżącym katalogu
file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_executable(AmebaRurki ${SOURCES})
