add_library(${FUNFS_PLATFORM}
	${CMAKE_CURRENT_SOURCE_DIR}/simulator/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/simulator/flash_emu.c
	${CMAKE_CURRENT_SOURCE_DIR}/simulator/simu_udp.c
)

target_include_directories(${FUNFS_PLATFORM}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/simulator
	PUBLIC ${PROJECT_SOURCE_DIR}/src
)

if(WIN32)
    target_link_libraries(${FUNFS_PLATFORM} PUBLIC ws2_32)
endif()