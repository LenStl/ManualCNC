cmake_minimum_required(VERSION 3.21.1)
project(ManualCNC)


set(SOURCES 	src/main.cpp 
		src/AssemblyInfo.cpp 
		src/pch.cpp 
		include/pch.h 
		include/smc5d.core.tlb
		include/smc5d.core.tlh
		include/WinUSBNet.tlb
		bin/smc5d/smc5d.core.dll
		bin/smc5d/WinUSBNet.dll)

add_executable(manual_cnc ${SOURCES})

target_sources(manual_cnc PRIVATE
	src/main.cpp
	src/AssemblyInfo.cpp
	src/pch.cpp
	)
target_include_directories(manual_cnc PUBLIC include/ PRIVATE src/)


set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT manual_cnc)
set_target_properties(manual_cnc PROPERTIES COMMON_LANGUAGE_RUNTIME "")