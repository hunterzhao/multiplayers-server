set(SRC_SOURCE
   tcpserver.cpp
   )

add_library(tcpserver STATIC ${SRC_SOURCE})
target_link_libraries(tcpserver)