cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME dhcpd)
set(DHCP_NETWORK dhcp-network)

add_executable(${PROJECT_NAME}
    src/dhcpd.c
)

target_link_libraries(${PROJECT_NAME}
    cunit
    lib::dhcpnetwork
    lib::pkt
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(${DHCP_NETWORK})