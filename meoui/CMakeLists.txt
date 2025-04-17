set(IMGUI_NODE_EDITOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/external/imgui_node_editor)
message("IMGUI_NODE_EDITOR_ROOT_DIR: ${IMGUI_NODE_EDITOR_ROOT_DIR}")

list(APPEND CMAKE_MODULE_PATH ${IMGUI_NODE_EDITOR_ROOT_DIR}/misc/cmake-modules)
find_package(imgui REQUIRED)
find_package(imgui_node_editor REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/application)
file(GLOB CommonResources "${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/data/*")
file(GLOB Resources "${CMAKE_CURRENT_SOURCE_DIR}/data/*")

set(Sources
        blueprints-example.cpp
        utilities/builders.h
        utilities/drawing.h
        utilities/widgets.h
        utilities/builders.cpp
        utilities/drawing.cpp
        utilities/widgets.cpp)
set(Name blueprints-example)
add_executable(${Name} ${Sources} ${CommonResources} ${Resources})
target_link_libraries(${Name} PRIVATE imgui imgui_node_editor application)
# copy resources
set(BinDir ${CMAKE_BINARY_DIR}/${Name})
add_custom_command(
        TARGET ${Name}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ARGS ${BinDir}/data
)
set(_ResourceRoot ${CMAKE_CURRENT_SOURCE_DIR})
foreach (_Resource
        ROOT "${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/data" ${CommonResources}
        ROOT "${CMAKE_CURRENT_SOURCE_DIR}/data" ${Resources})
#    MESSAGE("_Resource: ${_Resource}")
    if (_Resource STREQUAL ROOT)
        set(_ResourceRoot FALSE)
        continue()
    elseif(NOT _ResourceRoot)
        set(_ResourceRoot ${_Resource})
        continue()
    endif()

    file(RELATIVE_PATH _RelResource ${_ResourceRoot} ${_Resource})
    MESSAGE("_RelResource: ${_RelResource}")
    add_custom_command(
            TARGET ${Name}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ARGS ${_Resource} ${BinDir}/data/${_RelResource}
    )
endforeach()

file(GLOB Sources
        editor.cpp
        utilities/builders.h
        utilities/drawing.h
        utilities/widgets.h
        utilities/builders.cpp
        utilities/drawing.cpp
        utilities/widgets.cpp
        window/*.h
        window/*.cpp
)
set(Name editor)
add_executable(${Name} ${Sources} ${CommonResources} ${Resources})
target_link_libraries(${Name} PRIVATE imgui imgui_node_editor application)
# copy resources
set(BinDir ${CMAKE_BINARY_DIR}/${Name})
add_custom_command(
        TARGET ${Name}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ARGS ${BinDir}/data
)
set(_ResourceRoot ${CMAKE_CURRENT_SOURCE_DIR})
foreach (_Resource
        ROOT "${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/data" ${CommonResources}
        ROOT "${CMAKE_CURRENT_SOURCE_DIR}/data" ${Resources})
    #    MESSAGE("_Resource: ${_Resource}")
    if (_Resource STREQUAL ROOT)
        set(_ResourceRoot FALSE)
        continue()
    elseif(NOT _ResourceRoot)
        set(_ResourceRoot ${_Resource})
        continue()
    endif()

    file(RELATIVE_PATH _RelResource ${_ResourceRoot} ${_Resource})
    MESSAGE("_RelResource: ${_RelResource}")
    add_custom_command(
            TARGET ${Name}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ARGS ${_Resource} ${BinDir}/data/${_RelResource}
    )
endforeach()


set(Sources
        simple-example.cpp
        utilities/builders.h
        utilities/drawing.h
        utilities/widgets.h
        utilities/builders.cpp
        utilities/drawing.cpp
        utilities/widgets.cpp)
set(Name simple)
add_executable(${Name} ${Sources} ${CommonResources} ${Resources})
target_link_libraries(${Name} PRIVATE imgui imgui_node_editor application)
# copy resources
set(BinDir ${CMAKE_BINARY_DIR}/${Name})
add_custom_command(
        TARGET ${Name}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ARGS ${BinDir}/data
)
set(_ResourceRoot ${CMAKE_CURRENT_SOURCE_DIR})
foreach (_Resource
        ROOT "${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/data" ${CommonResources}
        ROOT "${CMAKE_CURRENT_SOURCE_DIR}/data" ${Resources})
    #    MESSAGE("_Resource: ${_Resource}")
    if (_Resource STREQUAL ROOT)
        set(_ResourceRoot FALSE)
        continue()
    elseif(NOT _ResourceRoot)
        set(_ResourceRoot ${_Resource})
        continue()
    endif()

    file(RELATIVE_PATH _RelResource ${_ResourceRoot} ${_Resource})
    MESSAGE("_RelResource: ${_RelResource}")
    add_custom_command(
            TARGET ${Name}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ARGS ${_Resource} ${BinDir}/data/${_RelResource}
    )
endforeach()