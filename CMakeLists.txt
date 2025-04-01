cmake_minimum_required(VERSION 3.5)
project(communication)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-std=c++11)
add_compile_options(-fPIC)

SET(INCLUDE_DIR

    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
include_directories(${INCLUDE_DIR})

SET(LIB_DIR

    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
link_directories(${LIB_DIR})

SET(INCLUDE_FILES 

    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}.h
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}.cpp ${INCLUDE_FILES})

set(sublib_list

	udp_communication
	rs485_communication
	tcpip_communication
    can_communication
    )
foreach(var ${sublib_list})
    add_library(${var} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/${var}.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/${var}.h)
endforeach()

SET(test_list

    test
	test_udp
	test_rs485
    test_can
    test_tcpip
    )
foreach(var ${test_list})
    add_executable(${CMAKE_PROJECT_NAME}_${var} ${CMAKE_CURRENT_SOURCE_DIR}/example/${var}.cpp)

    target_link_libraries(${CMAKE_PROJECT_NAME}_${var} udp_communication rs485_communication tcpip_communication can_communication communication controlcan pthread)
endforeach()

add_custom_target(copyfile ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/lib*.so ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "copy head files to include diretory; copy lib files to lib directory"
)

add_dependencies(copyfile ${CMAKE_PROJECT_NAME})
foreach(var ${sublib_list})
    add_dependencies(copyfile ${var})
endforeach()
