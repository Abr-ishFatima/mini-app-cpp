cmake_minimum_required(version 3.15)
project(NotesApp)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

add_executable(notes
main.cpp
database.cpp
notes.cpp)

target_link_libraries(notes SQLite3::SQLite3)