set(SERVER_SOURCE testtcpserver.cpp)
set(CLIENT_SOURCE client.cpp)

add_executable(server ${SERVER_SOURCE})
target_link_libraries(server tcpserver)

add_executable(client ${CLIENT_SOURCE})
