cmake_minimum_required(VERSION 3.27)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Meodini  LANGUAGES CXX C)

include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/winrt")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/cppwinrt")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared")




#add_subdirectory(meo)
add_subdirectory(meoui)



