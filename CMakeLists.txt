cmake_minimum_required(VERSION 3.25)
project(FunFS VERSION 0.1 LANGUAGES C)

set(FUNFS_SRC       "FunFS_src")
set(FUNFS_PLATFORM  "FunFS_platfrom")

if(WIN32)
    set(FUNFS_EXEC      "FunFS")
else()
    set(FUNFS_EXEC      "FunFS_exe")
endif()

#-------------------------- Choose required options

option(USE_DEBUG "Apply debugging flags" YES)
option(BUILD_32BIT "Build 32-bit version" YES)

add_compile_definitions(
	"PAGE_SIZE=64"
	"PAGES_TOTAL=64"
	"ITABLE_MEMORY_OCCUPATION_IN_PERCENTS=50"
	"DF_CHILDREN_LIST_LEN=10"
)

#-------------------------- Compiler and linker flags
if(BUILD_32BIT AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
endif()

if (USE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -g")
    if(NOT WIN32)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -static-libasan")
    endif()
    add_compile_definitions("FUNFS_DBG_MODE")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Ofast")
endif()

# Windows-specific runtime linking aimed to make .exe file run withiout external DLL dependencies
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

#-------------------------- Subdirectories and targets
add_subdirectory(src)
add_subdirectory(src/platform)

add_executable(${FUNFS_EXEC}
	main.c
	# src/tests_main.c
)

target_link_libraries(${FUNFS_EXEC}
	PUBLIC FunFS_src
	PUBLIC FunFS_platfrom
)

# linking Winsock2 (Windows-specific)
if(WIN32)
    target_link_libraries(${FUNFS_EXEC} PRIVATE ws2_32)
endif()

target_include_directories(${FUNFS_EXEC}
	PUBLIC ${PROJECT_SOURCE_DIR}/src
	PUBLIC ${PROJECT_SOURCE_DIR}/src/platform
)