add_executable(testsdl EXCLUDE_FROM_ALL test_sdl.cpp)
add_executable(teststate EXCLUDE_FROM_ALL test_state.cpp)
add_executable(testevent EXCLUDE_FROM_ALL test_event.cpp)
add_executable(testentity EXCLUDE_FROM_ALL test_entity.cpp)
add_executable(testai EXCLUDE_FROM_ALL test_ai.cpp)

target_link_libraries(testsdl Beaver)
target_link_libraries(teststate Beaver)
target_link_libraries(testevent Beaver)
target_link_libraries(testentity Beaver)
target_link_libraries(testai Beaver)

