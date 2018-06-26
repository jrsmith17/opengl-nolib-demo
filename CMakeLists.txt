cmake_minimum_required (VERSION 3.7)
project (OpenGL_NoLib_Demo)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	# Allows windows to create DLLs without using declspec
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(WIN32)
	
set (CMAKE_CXX_STANDARD 11)

# set build destionations
set(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib CACHE PATH "Build directory" FORCE)

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir})
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# add the binary tree to the search path for include files
include_directories(${PROJECT_SOURCE_DIR}/include)

set(COMMON_HEADERS
	include/render.h
	)

set(COMMON_SOURCE 
	)

if(WIN32)
	set(WIN32_HEADERS
		${COMMON_HEADERS}
		)

	set(WIN32_SOURCE
		${COMMON_SOURCE}
		src/win32_main.cpp)

	source_group(Headers FILES ${WIN32_HEADERS})
	source_group(Source FILES ${WIN32_SOURCE})
	
	# add the executable
	add_executable(OpenGL_NoLib_Demo WIN32 ${WIN32_HEADERS} ${WIN32_SOURCE})
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OpenGL_NoLib_Demo)
	target_link_libraries(OpenGL_NoLib_Demo opengl32)
endif()