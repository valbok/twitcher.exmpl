set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(UserTest UserTest.cpp)
target_link_libraries(UserTest gtest Twitcher)

add_executable(StorageTest StorageTest.cpp)
target_link_libraries(StorageTest gtest Twitcher)

add_executable(IndexerTest IndexerTest.cpp)
target_link_libraries(IndexerTest gtest Twitcher)

add_executable(TwitcherTest TwitcherTest.cpp)
target_link_libraries(TwitcherTest gtest Twitcher)
