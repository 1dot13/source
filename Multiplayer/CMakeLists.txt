add_library(Multiplayer
"client.cpp"
"server.cpp"
"transfer_rules.cpp"
)
target_include_directories(Multiplayer PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
"raknet"
)
target_link_libraries(Multiplayer PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/raknet/RakNetLibStatic.lib"
)
