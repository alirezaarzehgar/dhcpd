cmake_minimum_required(VERSION 3.6)

include(cmake/color.cmake)

set(PROJECT_NAME dhcpd)
set(DHCP_NETWORK dhcp-network)
set(DHCP_LEASE dhcp-lease)
set(HEADERS_DIR include/dhcpd)
set(INSTALL_DIR /usr/local/bin)
set(DHCP_LEASE_GENERATED_FAKE_DATA ${CMAKE_CURRENT_BINARY_DIR}/dhcp-lease/tests/databases/dhcpd.db)

option(DEBUG "debug mode" OFF)

if(DEBUG)
    set(DHCP_LEASE_DEST_FAKE_DATA ${CMAKE_CURRENT_BINARY_DIR}/dhcp-lease/tests/databases)
else()
    set(DHCP_LEASE_DEST_FAKE_DATA /var/lib/dhcp)       
endif()

add_definitions(-DDHCP_DATABASE_PATH="${DHCP_LEASE_DEST_FAKE_DATA}/dhcpd.db")

project(${PROJECT_NAME})

find_package(Git QUIET)

message("\n--------------------------------\n")
message("PROJECT NAME:\t\t${PROJECT_NAME}")
message("CMAKE_SYSTEM_NAME:\t${CMAKE_SYSTEM_NAME}")
message("CMAKE_C_COMPILER:\t${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER:\t${CMAKE_CXX_COMPILER}")
message("CMAKE_BINARY_DIR:\t${CMAKE_BINARY_DIR}")
message("CUNIT_LIBRARY:\t\t${CUNIT_LIBRARY}")
message("DEST_FAKE_DATA:\t\t${DHCP_LEASE_DEST_FAKE_DATA}")
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

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${INSTALL_DIR})

install(FILES ${DHCP_LEASE_GENERATED_FAKE_DATA}
        DESTINATION ${DHCP_LEASE_DEST_FAKE_DATA})

set(CPACK_GENERATOR "DEB")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alireza Arzehgar")

include(CPack)

add_subdirectory(${DHCP_NETWORK})
add_subdirectory(${DHCP_LEASE})

if(NOT DEBUG)
    if(NOT EXISTS "${DHCP_LEASE_DEST_FAKE_DATA}/dhcpd.db")
        message("\n")

        message(
            STATUS "${Red}${DHCP_LEASE_DEST_FAKE_DATA}/dhcpd.db not found!$"
        )
        message("${BoldBlue}If you want to use test and fake data enter following command on terminal")
        message("default database path is sets to \"${DHCP_LEASE_DEST_FAKE_DATA}/dhcpd.db\".${End}")
        message(
            STATUS "${Green}sudo cp ${DHCP_LEASE_GENERATED_FAKE_DATA} ${DHCP_LEASE_DEST_FAKE_DATA}${End}"
        )
        message("\n")
    endif()
endif()
