cmake_minimum_required(VERSION 2.8)
project (ToffanimEngine)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")


file(GLOB MAIN_INCLUDE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
       "src/*.cpp"
	   "src/*.h"
       "src/TECore/*.cpp"
	   "src/TECore/*.h"
       "src/TERenderer/*.cpp"
	   "src/TERenderer/*.h"
	   "src/TEPhysics/*.cpp"
	   "src/TEPhysics/*.h"
	   "src/TEGame/*.cpp"
	   "src/TEGame/*.h"
	   "src/TEEditor/*.cpp"
	   "src/TEEditor/*.h"
	   "src/TEDebug/*.cpp"
	   "src/TEDebug/*.h"
	   "src/main/*.cpp"
	   "src/main/*.h"
	   "src/utils/*.cpp"
	   "src/utils/*.h"
	   )
	   
add_executable(engine ${MAIN_INCLUDE})
assign_source_group(${MAIN_INCLUDE})
#GLFW
if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/CMakeLists.txt")
    # Could not find GLFW
    message("Unable to find GLFW")
	# Clone the submodule
	execute_process(COMMAND git submodule update --init -- lib/glfw
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Library GLFW detected")
endif()
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
add_subdirectory(lib/glfw)
set_property(TARGET glfw PROPERTY FOLDER GLFW)
target_link_libraries(engine glfw)

#Assimp
if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/CMakeLists.txt")
    # Could not find ASSIMP
    message("Unable to find ASSIMP")
	# Clone the submodule
	execute_process(COMMAND git submodule update --init -- lib/assimp
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Library ASSIMP detected")
endif()
option ( ASSIMP_BUILD_ASSIMP_TOOLS
  "If the supplementary tools for Assimp are built in addition to the library."
  OFF
)
OPTION ( ASSIMP_BUILD_TESTS
  "If the test suite for Assimp is built in addition to the library."
  OFF
)
add_subdirectory(lib/assimp)
set_property(TARGET assimp PROPERTY FOLDER ASSIMP)
if(TARGET zlibstatic)
set_property(TARGET zlibstatic PROPERTY FOLDER ASSIMP)
endif()
set_property(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTY FOLDER ASSIMP)
target_link_libraries(engine assimp)

#GLM
if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm/CMakeLists.txt")
    # Could not find ASSIMP
    message("Unable to find GLM")
	# Clone the submodule
	execute_process(COMMAND git submodule update --init -- lib/glm
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Library GLM detected")
endif()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")

#IMGUI
if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")
    # Could not find ASSIMP
    message("Unable to find IMGUI")
	# Clone the submodule
	execute_process(COMMAND git submodule update --init -- lib/imgui
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Library IMGUI detected")
endif()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")
file(GLOB IMGUI_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/examples/opengl3_example/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/examples/opengl3_example/*.h"
)
add_library( imgui STATIC ${IMGUI_SRC} )
target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/examples/libs/gl3w" 
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/examples/libs/glfw/include"	 
)
set_property(TARGET imgui PROPERTY FOLDER IMGUI)
target_link_libraries( imgui glew )
target_link_libraries( engine imgui )

#STB
if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb")
    # Could not find ASSIMP
    message("Unable to find STB")
	# Clone the submodule
	execute_process(COMMAND git submodule update --init -- lib/stb
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Library STB detected")
endif()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/stb")
file(GLOB STB_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb/*.c"
)
add_library( stb STATIC ${STB_SRC} )
set_property(TARGET stb PROPERTY FOLDER STB)
target_link_libraries( engine stb )


#EASTL
if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/EASTL/CMakeLists.txt")
    # Could not find EASTL
    message("Unable to find EASTL")
	# Clone the submodule
	execute_process(COMMAND git submodule update --init -- lib/EASTL
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Library EASTL detected")
endif()
add_subdirectory(lib/EASTL)
set_property(TARGET EASTL PROPERTY FOLDER EASTL)
set_property(TARGET EABase_ide PROPERTY FOLDER EASTL)
target_link_libraries(engine EASTL)

#GLEW
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/include")
file(GLOB GLEW_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/include/GL/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/include/GL/*.c"
)
add_definitions(-DGLEW_STATIC) 
add_library( glew STATIC ${GLEW_SRC})
set_property(TARGET glew PROPERTY FOLDER GLEW)
target_link_libraries( engine glew )
target_link_libraries( engine glu32 )
target_link_libraries( engine opengl32 )

set_property(TARGET uninstall PROPERTY FOLDER CMakePredefinedTargets)