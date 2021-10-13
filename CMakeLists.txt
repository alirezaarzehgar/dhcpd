cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME dhcpd)
set(DHCP_NETWORK dhcp-network)
set(DHCP_LEASE dhcp-lease)
set(HEADERS_DIR include/dhcpd)

project(${PROJECT_NAME})

find_package(Git QUIET)

message("\n--------------------------------\n")
message("PROJECT NAME:\t\t${PROJECT_NAME}")
message("CMAKE_SYSTEM_NAME:\t${CMAKE_SYSTEM_NAME}")
message("CMAKE_C_COMPILER:\t${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER:\t${CMAKE_CXX_COMPILER}")
message("CMAKE_BINARY_DIR:\t${CMAKE_BINARY_DIR}")
message("CUNIT_LIBRARY:\t\t${CUNIT_LIBRARY}")
message("\n--------------------------------\n")

if(GIT_FOUND AND EXISTS "../.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")

        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()    
endif()


if(TARGET DHCP_NETWORK)
    message("dhcp-network library not found")
endif()

if(TARGET DHCP_LEASE)
    message("dhcp-lease library not found")
endif()

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/${HEADERS_DIR}/config.h)

add_executable(${PROJECT_NAME}
    src/dhcpd.c
)

target_link_libraries(${PROJECT_NAME}
    lib::dhcpnetwork
    lib::pkt
    lib::lease
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(${DHCP_NETWORK})
add_subdirectory(${DHCP_LEASE})